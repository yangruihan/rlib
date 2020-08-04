#include "rlog.h"

#define MAX_CALLBACKS 32

typedef struct
{
    RLogLogFn fn;
    void*     udata;
    int       level;
} Callback;

static struct
{
    void*      udata;
    RLogLockFn lock;
    int        level;
    bool       quiet;
    Callback   callbacks[MAX_CALLBACKS];
} L;

static const char* levelStrings[] =
{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *levelColors[] =
{
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

static void stdoutCallback(LogEvent* ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
  fprintf(
    ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
    buf, levelColors[ev->level], levelStrings[ev->level],
    ev->file, ev->line);
#else
    fprintf(
        ev->udata, "%s %-5s %s:%d: ",
        buf, levelStrings[ev->level], ev->file, ev->line);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void fileCallback(LogEvent* ev)
{
    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    fprintf(
        ev->udata, "%s %-5s %s:%d: ",
        buf, levelStrings[ev->level], ev->file, ev->line);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void lock(void)
{
    if (L.lock) { L.lock(true, L.udata); }
}

static void unlock(void)
{
    if (L.lock) { L.lock(false, L.udata); }
}

static void initEvent(LogEvent* ev, void* udata)
{
    if (!ev->time)
    {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

/* ----- rlog ----- */

const char* rlog_levelString(const int level)
{
    return levelStrings[level];
}

void rlog_setLock(const RLogLockFn fn, void* udata)
{
    L.lock = fn;
    L.udata = udata;
}

void rlog_setLevel(const int level)
{
    L.level = level;
}

void rlog_setQuiet(const bool enable)
{
    L.quiet = enable;
}

int rlog_addCallback(RLogLogFn fn, void* udata, int level)
{
    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        if (!L.callbacks[i].fn)
        {
            L.callbacks[i] = (Callback) {fn, udata, level};
            return 0;
        }
    }
    return -1;
}

int rlog_addFp(FILE* fp, const int level)
{
    return rlog_addCallback(fileCallback, fp, level);
}

void rlog_log(const int level, const char* file, const int line, const char* fmt, ...)
{
    LogEvent ev = {
        .fmt = fmt,
        .file = file,
        .line = line,
        .level = level,
    };

    lock();

    if (!L.quiet && level >= L.level)
    {
        initEvent(&ev, stderr);
        va_start(ev.ap, fmt);
        stdoutCallback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++)
    {
        Callback* cb = &L.callbacks[i];
        if (level >= cb->level)
        {
            initEvent(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

/* ----- end rlog ----- */