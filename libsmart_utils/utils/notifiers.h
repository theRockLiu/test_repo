//============================================================================
// Name        : EventNotifier.h
// Author      : TheRockLiuHY
// Date        : Apr 29, 2015
// Copyright   : liuhongyang's copyright
// Description : SmartUtils
//============================================================================

#ifndef UTILS_NOTIFIERS_H_
#define UTILS_NOTIFIERS_H_

#include <signal.h>

#include <memory>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <string>

#include "../base/base.h"

namespace smart_utils
{

	class notifier_interface
	{
		public:
			typedef std::shared_ptr<smart_utils::notifier_interface> pointer_t;

		public:
			notifier_interface()
			{
			}

			virtual ~notifier_interface()
			{
			}

		public:
			virtual int32_t open(bool flag) = 0;
			virtual int32_t close() = 0;
			virtual void on_added(bool Suc)
			{
			}
			virtual void on_removed(bool Suc)
			{
			}
			virtual int32_t get_fd() = 0;
			virtual uint32_t get_events() = 0;
			virtual void handle_events(uint32_t evts) = 0;
	};

	class tcp_sock_accept_notifier: public notifier_interface
	{
			DISABLE_COPY(tcp_sock_accept_notifier)
			DISABLE_MOVE(tcp_sock_accept_notifier)
		public:
			typedef std::shared_ptr<smart_utils::tcp_sock_accept_notifier> pointer_t;
			tcp_sock_accept_notifier(std::string &bind_ip, uint_fast16_t bind_port);
			virtual ~tcp_sock_accept_notifier();

		public:
			int32_t open(bool nonblock);
			int32_t close();
			void on_added(bool Suc);
			void on_removed(bool Suc);
			int32_t get_fd();
			uint32_t get_events();
			void handle_events(uint32_t evts);
			virtual void handle_new_conn(int_fast32_t sock, const std::string &remote_ip, uint_fast16_t remote_port) = 0;

		private:
			int_fast32_t sock_;
			std::string bind_ip_;
			uint_fast16_t bind_port_;
	};

	class tcp_sock_notifier: public notifier_interface
	{
			DISABLE_COPY(tcp_sock_notifier)
			DISABLE_MOVE(tcp_sock_notifier)
		public:
			typedef std::shared_ptr<smart_utils::tcp_sock_notifier> pointer_t;
			tcp_sock_notifier(int_fast32_t sock);
			virtual ~tcp_sock_notifier();

		public:
			int32_t open(bool flag);
			int32_t close();
			void on_added(bool Suc);
			void on_removed(bool Suc);
			int32_t get_fd();
			uint32_t get_events();
			void handle_events(uint32_t evts);
			virtual void handle_input(byte_t *data, uint_fast32_t &offset, uint_fast32_t &len)
			{
			}
			virtual void handle_output()
			{
			}
			virtual void handle_close() = 0;

		private:
			int_fast32_t sock_;
#define MAX_RECV_BUF_SIZE (1024 * 1024)
			byte_t recv_buf_[MAX_RECV_BUF_SIZE];
			uint_fast32_t data_offset_;
			uint_fast32_t data_len_;
	};

	/**
	 * timer event handler
	 * */
	class timer_notifier: public notifier_interface
	{
			DISABLE_COPY(timer_notifier)
			DISABLE_MOVE(timer_notifier)

		public:
			typedef std::shared_ptr<smart_utils::timer_notifier> pointer_t;

		public:
			enum ETimerType
			{
				ETT_REALTIME = 0, ETT_MONOTONIC = 1
			};

		public:
			explicit timer_notifier(const ETimerType timer_type, int64_t interval_seconds, int64_t interval_nanos);
			virtual ~timer_notifier();

			int32_t open(bool flag);
			int32_t close();
			uint32_t get_events();
			void handle_events(uint32_t evts);
			int32_t get_fd()
			{
				return fd_;
			}

			virtual void handle_timeout(uint64_t times) = 0;

		public:
			int64_t get_interval_s()
			{
				return interval_s_;
			}

			int64_t get_interval_ns()
			{
				return interval_ns_;
			}

		private:
			int32_t fd_;
			int32_t timer_type_;
			int64_t init_expire_s_;
			int64_t init_expire_ns_;
			int64_t interval_s_;
			int64_t interval_ns_;
	};

	/**
	 *
	 *
	 * */
	class event_notifier: public notifier_interface
	{
			DISABLE_COPY(event_notifier)
			DISABLE_MOVE(event_notifier)

		public:
			typedef std::shared_ptr<smart_utils::event_notifier> pointer_t;

		public:
			explicit event_notifier();
			virtual ~event_notifier();

		public:
			int32_t open(bool nonblock);
			int32_t close();
			uint32_t get_events();
			void handle_events(uint32_t evts);
			int32_t get_fd()
			{
				return fd_;
			}
			void notify(uint64_t val);
			int_fast8_t wait_evt();

			virtual void handle_event(uint64_t val)
			{
			}

		private:
			int32_t fd_;
	};

	/**
	 *
	 *
	 * */
	class signal_notifier: public notifier_interface
	{
			DISABLE_COPY(signal_notifier)
			//DISABLE_MOVE(CSignalBase)

		public:
			typedef std::shared_ptr<smart_utils::signal_notifier> pointer_t;

		public:
			explicit signal_notifier(std::vector<int32_t> &&vec);
			virtual ~signal_notifier();

		public:
			int32_t open(bool flag);
			int32_t close();
			uint32_t get_events();
			void handle_events(uint32_t evts);
			int32_t get_fd()
			{
				return fd_;
			}

			virtual void handle_signal(int32_t sig) = 0;

		private:
			std::vector<int> signals_;
			int32_t fd_;
	};

	/**
	 *
	 *
	 * */
	class notifiers_engine
	{
			DISABLE_COPY(notifiers_engine)
			DISABLE_MOVE(notifiers_engine)

		public:
			typedef std::shared_ptr<smart_utils::notifiers_engine> pointer_t;

		public:
			notifiers_engine();
			~notifiers_engine();

		public:
			///not thread-safe.
			int32_t open();
			///not thread-safe
			int32_t close();
			///
			bool is_opened();
			void async_add_notifier(notifier_interface::pointer_t &pEvtHandler);
			void async_rem_notifier(notifier_interface::pointer_t &pEvtHandler);
			void check_once(int32_t TimeoutMS = -1);
		private:
			///
			typedef std::unordered_set<smart_utils::notifier_interface::pointer_t> notifiers_t;
			notifiers_t notifiers_;

			///
			typedef std::vector<smart_utils::notifier_interface::pointer_t> tmp_notifiers_t;
			std::mutex tmp_notifier_add_mtx_;
			tmp_notifiers_t tmp_add_notifiers_;
			std::mutex tmp_notifier_remove_mtx_;
			tmp_notifiers_t tmp_remove_notifiers_;

			///
			int32_t epfd_;
	};

} /* namespace NSSmartUtils */

#endif /* UTILS_NOTIFIERS_H_ */
