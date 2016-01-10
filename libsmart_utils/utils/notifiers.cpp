//============================================================================
// Name        : EventNotifier.cpp
// Author      : TheRockLiuHY
// Date        : Apr 29, 2015
// Copyright   : liuhongyang's copyright
// Description : SmartUtils
//============================================================================

#include <sys/epoll.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "notifiers.h"

namespace smart_utils
{

	notifiers_engine::notifiers_engine()
											: epfd_(-1)
	{
	}

	notifiers_engine::~notifiers_engine()
	{
	}

	void notifiers_engine::async_add_notifier(notifier_interface::pointer_t& pEvtHandler)
	{
		std::lock_guard<std::mutex> lock(tmp_notifier_add_mtx_);
		if (!is_opened())
		{
			SU_CHECK(false);
			return;
		}
		tmp_add_notifiers_.push_back(pEvtHandler);
	}

	void notifiers_engine::async_rem_notifier(notifier_interface::pointer_t& pEvtHandler)
	{
		std::lock_guard<std::mutex> lock(tmp_notifier_remove_mtx_);
		if (!is_opened())
		{
			SU_CHECK(false);
			return;
		}
		tmp_remove_notifiers_.push_back(pEvtHandler);
	}

#define MAX_EVENTS (100)
	void notifiers_engine::check_once(int32_t TimeoutMS)
	{
		if (!is_opened())
		{
			SU_CHECK(false);
			return;
		}

		if (!tmp_remove_notifiers_.empty())
		{
			std::vector<notifier_interface::pointer_t> TmpVec;

			{
				std::lock_guard<std::mutex> lock(tmp_notifier_remove_mtx_);
				tmp_remove_notifiers_.swap(TmpVec);
			}
			SU_CHECK(tmp_remove_notifiers_.empty());

			for (tmp_notifiers_t::iterator iter = TmpVec.begin(); iter != TmpVec.end(); iter++)
			{
				///In  kernel  versions  before 2.6.9, the EPOLL_CTL_DEL operation required a non-null pointer in event, even though this argument is ignored.
				///Since Linux 2.6.9, event can be specified as NULL when using EPOLL_CTL_DEL.  Applications that need to be portable to kernels before  2.6.9
				///should specify a non-null pointer in event.
				notifiers_t::size_type st = notifiers_.erase(*iter);
				SU_CHECK(1 == st);
				(*iter)->on_removed(0 == st ? false : (epoll_ctl(epfd_, EPOLL_CTL_DEL, (*iter)->get_fd(),
				NULL) == 0));
			}
		}

		if (!tmp_add_notifiers_.empty())
		{
			tmp_notifiers_t tmp_add_notifiers;

			{
				std::lock_guard<std::mutex> lock(tmp_notifier_add_mtx_);
				tmp_add_notifiers_.swap(tmp_add_notifiers);
			}
			SU_CHECK(tmp_add_notifiers_.empty());

			for (tmp_notifiers_t::iterator iter = tmp_add_notifiers.begin(); iter != tmp_add_notifiers.end(); iter++)
			{
				std::pair<notifiers_t::iterator, bool> ret = notifiers_.insert(*iter);
				SU_CHECK(ret.second);
				struct epoll_event evt = { (*iter)->get_events(), { (*iter).get() }, };
				(*iter)->on_added((!ret.second) ? false : (epoll_ctl(epfd_, EPOLL_CTL_ADD, (*iter)->get_fd(), &evt) == 0));			//you see, i wanna you get the errno:)...
			}
		}

		struct epoll_event events[MAX_EVENTS];

		int32_t nfds = epoll_wait(epfd_, events, MAX_EVENTS, TimeoutMS);
		if (nfds == -1)
		{
			SU_CHECK(false);
			return;
		}

		for (int32_t n = 0; n < nfds; ++n)
		{
			notifier_interface* pHandler = static_cast<notifier_interface*>(events[n].data.ptr);
			SU_CHECK(NULL != pHandler);
			pHandler->handle_events(events[n].events);
		}

	}

	const int64_t NANOS_OF_ONE_SECONDS = (1000 * 1000 * 1000);

	timer_notifier::timer_notifier(const ETimerType tt, int64_t interval_seconds, int64_t interval_nanos)
											: fd_(-1), timer_type_(tt), init_expire_s_(interval_seconds), init_expire_ns_(interval_nanos), interval_s_(interval_seconds), interval_ns_(interval_nanos)
	{

	}

	timer_notifier::~timer_notifier()
	{
		if (-1 == fd_)
		{
			::close(fd_);
		}
	}

	int32_t timer_notifier::open(bool flag)
	{

		SU_CHECK(init_expire_ns_ < NANOS_OF_ONE_SECONDS && interval_ns_ < NANOS_OF_ONE_SECONDS);

		if (ETT_REALTIME != timer_type_ && ETT_MONOTONIC != timer_type_)
		{
			return SU_EC_ERR;
		}

		int32_t timer_type = ETT_REALTIME == timer_type_ ? CLOCK_REALTIME : CLOCK_MONOTONIC;

		struct timespec now = { 0 };
		int32_t flags = 0;
#if (__ABS_TIME__)
		if (clock_gettime(timer_type, &now) == -1)
		{
			return SU_EC_ERR;
		}
		flags = TFD_TIMER_ABSTIME;
#endif

		struct itimerspec new_value = { 0 };
		new_value.it_value.tv_sec = now.tv_sec + init_expire_s_ + (now.tv_nsec + init_expire_ns_) / NANOS_OF_ONE_SECONDS;
		new_value.it_value.tv_nsec = (now.tv_nsec + init_expire_ns_) % NANOS_OF_ONE_SECONDS;
		new_value.it_interval.tv_sec = interval_s_ + interval_ns_ / NANOS_OF_ONE_SECONDS;
		new_value.it_interval.tv_nsec = interval_ns_ % NANOS_OF_ONE_SECONDS;

		fd_ = timerfd_create(timer_type, 0);
		if (fd_ == -1)
		{
			return SU_EC_ERR;
		}

		if (timerfd_settime(fd_, flags, &new_value, NULL) == -1)
		{
			return SU_EC_ERR;
		}

		return SU_EC_SUC;

	}

	int32_t timer_notifier::close()
	{
		SU_CHECK(-1 == fd_);
		::close(fd_);
		fd_ = -1;

		return SU_EC_ERR;
	}

	uint32_t timer_notifier::get_events()
	{
		return EPOLLIN;
	}

	void timer_notifier::handle_events(uint32_t events)
	{
		SU_CHECK(EPOLLIN == events);

		uint64_t times = 0;
		ssize_t s = read(fd_, &times, sizeof(uint64_t));
		if (s != sizeof(uint64_t))
		{
			SU_CHECK(false);
			return;
		}

		handle_timeout(times);
	}

	int32_t notifiers_engine::open()
	{
		std::lock_guard<std::mutex> A(tmp_notifier_remove_mtx_);
		std::lock_guard<std::mutex> B(tmp_notifier_add_mtx_);

		if (is_opened())
		{
			return SU_EC_REDO_ERR;
		}

		SU_CHECK(tmp_add_notifiers_.empty() && tmp_remove_notifiers_.empty() && notifiers_.empty());
		/*
		 * In the initial epoll_create() implementation, the size argument informed the kernel of the number  of  file  descriptors  that  the  caller
		 expected  to add to the epoll instance.  The kernel used this information as a hint for the amount of space to initially allocate in inter鈥�
		 nal data structures describing events.  (If necessary, the kernel would allocate more space if the caller's usage exceeded the  hint  given
		 in  size.)  Nowadays, this hint is no longer required (the kernel dynamically sizes the required data structures without needing the hint),
		 but size must still be greater than zero, in order to ensure backward compatibility when new epoll applications are run on older kernels.
		 * */
		return ((epfd_ = epoll_create(MAX_EVENTS)) == -1 ? SU_EC_ERR : SU_EC_SUC);
	}

	int32_t notifiers_engine::close()
	{
		std::lock_guard<std::mutex> A(tmp_notifier_remove_mtx_);
		std::lock_guard<std::mutex> B(tmp_notifier_add_mtx_);

		if (!is_opened())
		{
			return SU_EC_ERR;
		}

		tmp_add_notifiers_.clear();
		tmp_remove_notifiers_.clear();
		notifiers_.clear();

		SAFE_CLOSE_FD(epfd_)

		return SU_EC_SUC;
	}

	bool notifiers_engine::is_opened()
	{
		return !(-1 == epfd_);
	}

	event_notifier::event_notifier()
											: fd_(-1)
	{
	}

	event_notifier::~event_notifier()
	{
		close();
	}

	int32_t event_notifier::open(bool nonblock)
	{
		return (fd_ = eventfd(0, nonblock ? EFD_NONBLOCK : 0)) == -1 ? SU_EC_ERR : SU_EC_SUC;
	}

	int32_t event_notifier::close()
	{
		if (-1 != fd_)
		{
			::close(fd_);
			fd_ = -1;

			return SU_EC_SUC;
		}

		return SU_EC_ERR;
	}

	uint32_t event_notifier::get_events()
	{
		return EPOLLIN;
	}

	void event_notifier::notify(uint64_t val)
	{
		write(fd_, &val, sizeof(uint64_t));
	}

	void event_notifier::handle_events(uint32_t evts)
	{
		SU_CHECK(EPOLLIN == evts);

		uint64_t val = 0;
		ssize_t s = read(fd_, &val, sizeof(uint64_t));
		if (s != sizeof(uint64_t))
		{
			SU_CHECK(false);
			return;
		}

		handle_event(val);
	}

	int_fast8_t event_notifier::wait_evt()
	{
		uint64_t val = 0;
		ssize_t s = read(fd_, &val, sizeof(uint64_t));

		return s < 0 ? -1 : 0;
	}

	signal_notifier::signal_notifier(std::vector<int32_t> &&vec)
											: signals_(vec)
	{
	}

	signal_notifier::~signal_notifier()
	{
	}

	int32_t signal_notifier::open(bool flag)
	{
		sigset_t mask;
		sigemptyset(&mask);
		SU_CHECK(!signals_.empty());
		for (std::vector<int32_t>::iterator iter = signals_.begin(); iter != signals_.end(); iter++)
		{
			sigaddset(&mask, *iter);
			//std::cout<<*iter<<std::endl;
		}

		if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
		{
			SU_CHECK(false);
			return SU_EC_ERR;
		}

		fd_ = signalfd(-1, &mask, SFD_NONBLOCK);
		if (fd_ == -1)
		{
			SU_CHECK(false);
			return SU_EC_ERR;
		}

		return SU_EC_SUC;
	}

	int32_t signal_notifier::close()
	{
		SAFE_CLOSE_FD(fd_)

		return SU_EC_SUC;
	}

	uint32_t signal_notifier::get_events()
	{
		return EPOLLIN;
	}

	void signal_notifier::handle_events(uint32_t evts)
	{
		//std::cout<<"get signal"<<std::endl;
		struct signalfd_siginfo fdsi;
		ssize_t s;
		for (;;)
		{
			s = read(fd_, &fdsi, sizeof(struct signalfd_siginfo));
			if (s != sizeof(struct signalfd_siginfo))
			{
				SU_CHECK(EAGAIN == errno);
				break;
			}

			handle_signal(fdsi.ssi_signo);
		}
	}

	int32_t tcp_sock_accept_notifier::open(bool nonblock)
	{
		sock_ = socket(AF_INET, SOCK_STREAM | (nonblock ? SOCK_NONBLOCK : 0), 0);
		if (-1 == sock_)
		{
			return -1;
		}

		struct sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(bind_port_);
		addr.sin_addr.s_addr = inet_addr(bind_ip_.c_str());
		if (bind(sock_, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		{
			return -2;
		}

#define LISTEN_BACKLOG (10)
		if (listen(sock_, LISTEN_BACKLOG) < 0)
		{
			return -3;
		}

		return 0;
	}

	int32_t tcp_sock_accept_notifier::close()
	{
		::close(sock_);
		sock_ = -1;

		return 0;
	}

	void tcp_sock_accept_notifier::on_added(bool Suc)
	{
		SU_CHECK(Suc);
	}

	void tcp_sock_accept_notifier::on_removed(bool Suc)
	{
		SU_CHECK(Suc);
	}

	int32_t tcp_sock_accept_notifier::get_fd()
	{
		return sock_;
	}

	uint32_t tcp_sock_accept_notifier::get_events()
	{
		return EPOLLIN;
	}

	tcp_sock_accept_notifier::tcp_sock_accept_notifier(std::string &bind_ip, uint_fast16_t bind_port)
											: sock_(-1), bind_ip_(bind_ip), bind_port_(bind_port)
	{
	}

	tcp_sock_accept_notifier::~tcp_sock_accept_notifier()
	{
		close();
	}

	void tcp_sock_accept_notifier::handle_events(uint32_t evts)
	{
		SU_CHECK(EPOLLIN == evts);
		sockaddr_in addr = { 0 };
		socklen_t len = sizeof(addr);
		int_fast32_t sock = -1;
		sock = accept4(sock_, (struct sockaddr *) &addr, &len, SOCK_NONBLOCK);

		handle_new_conn(sock, std::string(inet_ntoa(addr.sin_addr)), ntohs(addr.sin_port));
	}

	tcp_sock_notifier::tcp_sock_notifier(int_fast32_t sock)
											: sock_(sock)
	{
	}

	tcp_sock_notifier::~tcp_sock_notifier()
	{
		close();
	}

	int32_t tcp_sock_notifier::open(bool flag)
	{
		SU_CHECK(sock_ >= 0);

		return 0;
	}

	int32_t tcp_sock_notifier::close()
	{
		SAFE_CLOSE_FD(sock_);

		return 0;
	}

	void tcp_sock_notifier::on_added(bool Suc)
	{
		SU_CHECK(Suc);
	}

	void tcp_sock_notifier::on_removed(bool Suc)
	{
		SU_CHECK(Suc);
	}

	int32_t tcp_sock_notifier::get_fd()
	{
		return sock_;
	}

	uint32_t tcp_sock_notifier::get_events()
	{
		return EPOLLIN;
	}

	void tcp_sock_notifier::handle_events(uint32_t evts)
	{
		SU_CHECK(EPOLLIN == evts);
		///TheRock: very ugly, but u know i will todo it.
		int_fast32_t ret = recv(sock_, recv_buf_ + data_offset_,
		MAX_RECV_BUF_SIZE - data_offset_ - data_len_, 0);
		if (-1 == ret)
		{
			handle_close();
			return;
		}
		data_len_ += ret;
		handle_input(recv_buf_ + data_offset_, data_offset_, data_len_);
		memmove(recv_buf_, recv_buf_ + data_offset_, data_len_);
		data_offset_ = 0;
	}
/* namespace NSSmartUtils */

}
