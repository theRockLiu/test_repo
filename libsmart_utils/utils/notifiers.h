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

class notifier
{
public:
	typedef std::shared_ptr<smart_utils::notifier> pointer_t;

public:
	notifier()
	{
	}

	virtual ~notifier()
	{
	}

public:
	virtual int32_t open() = 0;
	virtual int32_t close() = 0;
	virtual void on_added(bool Suc) = 0;
	virtual void on_removed(bool Suc) = 0;
	virtual int32_t get_fd() = 0;
	virtual uint32_t get_events() = 0;
	virtual void handle_events(uint32_t evts) = 0;
};

class tcp_sock_accepter: public notifier
{
	DISABLE_COPY(tcp_sock_accepter)
	DISABLE_MOVE(tcp_sock_accepter)
public:
	typedef std::shared_ptr<smart_utils::tcp_sock_accepter> pointer_t;
	tcp_sock_accepter(std::string &bind_ip, uint_fast16_t bind_port);
	virtual ~tcp_sock_accepter();

public:
	int32_t open();
	int32_t close();
	void on_added(bool Suc);
	void on_removed(bool Suc);
	int32_t get_fd();
	uint32_t get_events();
	void handle_events(uint32_t evts);
	virtual void handle_new_conn(int_fast32_t sock,
			const std::string &remote_ip, uint_fast16_t remote_port) = 0;

private:
	int_fast32_t sock_;
	std::string bind_ip_;
	uint_fast16_t bind_port_;
};

class tcp_sock: public notifier
{
	DISABLE_COPY(tcp_sock)
	DISABLE_MOVE(tcp_sock)
public:
	typedef std::shared_ptr<smart_utils::tcp_sock> pointer_t;
	tcp_sock(int_fast32_t sock);
	virtual ~tcp_sock();

public:
	int32_t open();
	int32_t close();
	void on_added(bool Suc);
	void on_removed(bool Suc);
	int32_t get_fd();
	uint32_t get_events();
	void handle_events(uint32_t evts);
	virtual void handle_input(byte_t *data, uint_fast32_t &offset, uint_fast32_t &len){}
	virtual void handle_output() {}
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
class timer_base: public notifier
{
	DISABLE_COPY(timer_base)
	DISABLE_MOVE(timer_base)

public:
	typedef std::shared_ptr<smart_utils::timer_base> pointer_t;

public:
	enum ETimerType
	{
		ETT_REALTIME = 0, ETT_MONOTONIC = 1
	};

public:
	explicit timer_base(const ETimerType timer_type, int64_t interval_seconds,
			int64_t interval_nanos);
	virtual ~timer_base();

	int32_t open();
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
class event_base: public notifier
{
	DISABLE_COPY(event_base)
	DISABLE_MOVE(event_base)

public:
	typedef std::shared_ptr<smart_utils::event_base> pointer_t;

public:
	explicit event_base();
	virtual ~event_base();

public:
	int32_t open();
	int32_t close();
	uint32_t get_events();
	void handle_events(uint32_t evts);
	int32_t get_fd()
	{
		return fd_;
	}
	void notify(uint64_t val);

	virtual void handle_event(uint64_t val) = 0;

private:
	int32_t fd_;
};

/**
 *
 *
 * */
class signal_base: public notifier
{
	DISABLE_COPY(signal_base)
	//DISABLE_MOVE(CSignalBase)

public:
	typedef std::shared_ptr<smart_utils::signal_base> pointer_t;

public:
	explicit signal_base(std::vector<int32_t> &&vec);
	virtual ~signal_base();

public:
	int32_t open();
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
typedef std::shared_ptr<smart_utils::signal_base> signal_base_ptr_t;

/**
 *
 *
 * */
class notifier_engine
{
	DISABLE_COPY(notifier_engine)
	DISABLE_MOVE(notifier_engine)

public:
	typedef std::shared_ptr<smart_utils::notifier_engine> pointer_t;

public:
	notifier_engine();
	~notifier_engine();

public:
	///not thread-safe.
	int32_t open();
	///not thread-safe
	int32_t close();
	///
	bool is_opened();
	void async_add_notifier(notifier::pointer_t &pEvtHandler);
	void async_remove_notifier(notifier::pointer_t &pEvtHandler);
	void check_once(int32_t TimeoutMS = -1);
private:
	///
	typedef std::unordered_set<smart_utils::notifier::pointer_t> notifiers_t;
	notifiers_t notifiers_;

	///
	typedef std::vector<smart_utils::notifier::pointer_t> tmp_notifiers_t;
	std::mutex tmp_notifier_add_mtx_;
	tmp_notifiers_t tmp_add_notifiers_;
	std::mutex tmp_notifier_remove_mtx_;
	tmp_notifiers_t tmp_remove_notifiers_;

	///
	int32_t epfd_;
};

} /* namespace NSSmartUtils */

#endif /* UTILS_NOTIFIERS_H_ */
