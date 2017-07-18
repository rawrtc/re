/**
 * @file re_main.h  Interface to main polling routine
 *
 * Copyright (C) 2010 Creytiv.com
 */
// #include <uv.h>
#include <uv.h>
#include "re_tmr.h"
#include "re_list.h"

enum {
#ifndef FD_READ
	FD_READ   = 1<<0,
#endif
#ifndef FD_WRITE
	FD_WRITE  = 1<<1,
#endif
	FD_EXCEPT = 1<<2
};

enum external_loop {
	LOOP_NONE = 0,
	LOOP_UV
};


/**
 * File descriptor event handler
 *
 * @param flags  Event flags
 * @param arg    Handler argument
 */
typedef void (fd_h)(int flags, void *arg);

/**
 * Thread-safe signal handler
 *
 * @param sig Signal number
 */
typedef void (re_signal_h)(int sig);


int   fd_listen(int fd, int flags, fd_h *fh, void *arg);
void  fd_close(int fd);
int   fd_setsize(int maxfds);
void  fd_debug(void);

int   libre_init(void);
void  libre_close(void);

int   re_main(re_signal_h *signalh);
void  re_cancel(void);
int   re_debug(struct re_printf *pf, void *unused);

int  re_thread_init(void);
void re_thread_close(void);
void re_thread_enter(void);
void re_thread_leave(void);

void re_set_mutex(void *mutexp);

struct external_handle {
	struct le le;
	uv_handle_t *handle;
	uv_handle_type type;
	void *data;
};


/** Polling methods */
enum poll_method {
	METHOD_NULL = 0,
	METHOD_POLL,
	METHOD_SELECT,
	METHOD_EPOLL,
	METHOD_KQUEUE,
	/* sep */
	METHOD_MAX
};

int              poll_method_set(enum poll_method method);
enum poll_method poll_method_best(void);
const char      *poll_method_name(enum poll_method method);
int poll_method_type(enum poll_method *method, const struct pl *name);

/** Methods to handle external event loop */
void external_loop_set(enum external_loop loop, uv_loop_t *arg);
enum external_loop re_get_external_loop_type(void);
uv_loop_t* re_get_external_loop(void);
void uv_start_timer(struct tmr *tmr);
void init_external_handles(void);
int alloc_fds(int maxfds);
