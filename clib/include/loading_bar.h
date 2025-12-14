#ifndef LOADING_BAR_H
#define LOADING_BAR_H

/*
  loading_bar.h - single-header progress/loading bar for C

  Features:
    - Header-only (no .c)
    - Works with stdout/stderr
    - ANSI carriage-return based
    - Optional spinner mode
    - Can be disabled via LOADING_BAR_DISABLE
    - Attempts to avoid spamming when not a TTY

  Usage:
    lb_bar_t bar;
    lb_bar_init(&bar, 100, "Processing", stdout);
    for (size_t i=0; i<=100; i++) { lb_bar_update(&bar, i); ... }
    lb_bar_done(&bar);

  Optional:
    lb_spinner_t sp;
    lb_spinner_init(&sp, "Working", stdout);
    while (...) { lb_spinner_tick(&sp); }
    lb_spinner_done(&sp);
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifndef LOADING_BAR_DISABLE

/* ---------- platform-ish helpers ---------- */
static inline int lb__is_tty(FILE *out) {
  (void)out;
#if defined(_WIN32)
  /* Avoid extra deps; assume interactive. Many Windows terminals support \r. */
  return 1;
#else
  /* If you want strict TTY detection, define LOADING_BAR_ASSUME_TTY=1 or provide your own. */
  #ifdef LOADING_BAR_ASSUME_TTY
    return 1;
  #else
    /* Best-effort without unistd.h to keep header lightweight:
       - if you want real isatty, define LOADING_BAR_USE_ISATTY and include unistd.h before this header. */
    #ifdef LOADING_BAR_USE_ISATTY
      extern int isatty(int);
      extern int fileno(FILE*);
      return isatty(fileno(out));
    #else
      return 1;
    #endif
  #endif
#endif
}

static inline double lb__now_seconds(void) {
#if defined(CLOCK_MONOTONIC)
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#else
  return (double)clock() / (double)CLOCKS_PER_SEC;
#endif
}

/* ---------- progress bar ---------- */
typedef struct lb_bar_t {
  size_t total;
  size_t last_drawn;
  int width;
  const char *label;
  FILE *out;
  int enabled;
  double t0;
} lb_bar_t;

static inline void lb_bar_init(lb_bar_t *b, size_t total, const char *label, FILE *out) {
  b->total = (total == 0) ? 1 : total;
  b->last_drawn = (size_t)-1;
  b->width = 40;
  b->label = label ? label : "";
  b->out = out ? out : stdout;
  b->enabled = lb__is_tty(b->out);
  b->t0 = lb__now_seconds();
}

static inline void lb_bar_set_width(lb_bar_t *b, int width) {
  if (width < 10) width = 10;
  if (width > 200) width = 200;
  b->width = width;
}

static inline void lb_bar_update(lb_bar_t *b, size_t current) {
  if (!b->enabled) return;

  if (current > b->total) current = b->total;

  /* Avoid redrawing too often if total is big */
  if (b->last_drawn != (size_t)-1) {
    size_t step = b->total / 200; /* ~200 redraws max */
    if (step < 1) step = 1;
    if (current != b->total && (current - b->last_drawn) < step) return;
  }
  b->last_drawn = current;

  double pct = (double)current / (double)b->total;
  int filled = (int)(pct * (double)b->width);
  if (filled > b->width) filled = b->width;

  double elapsed = lb__now_seconds() - b->t0;
  double rate = (elapsed > 0.0) ? ((double)current / elapsed) : 0.0;
  double eta = (rate > 0.0) ? (((double)b->total - (double)current) / rate) : 0.0;

  fprintf(b->out, "\r%s%s[", b->label[0] ? b->label : "", b->label[0] ? " " : "");
  for (int i = 0; i < b->width; i++) fputc(i < filled ? '=' : ' ', b->out);
  fprintf(b->out, "] %3d%% (%zu/%zu)  %.1fs elapsed  %.1fs eta",
          (int)(pct * 100.0 + 0.5), current, b->total, elapsed, eta);
  fflush(b->out);
}

static inline void lb_bar_done(lb_bar_t *b) {
  if (!b->enabled) return;
  lb_bar_update(b, b->total);
  fputc('\n', b->out);
  fflush(b->out);
}

/* ---------- spinner ---------- */
typedef struct lb_spinner_t {
  const char *label;
  FILE *out;
  int enabled;
  unsigned idx;
  double t0;
} lb_spinner_t;

static inline void lb_spinner_init(lb_spinner_t *s, const char *label, FILE *out) {
  s->label = label ? label : "";
  s->out = out ? out : stdout;
  s->enabled = lb__is_tty(s->out);
  s->idx = 0;
  s->t0 = lb__now_seconds();
}

static inline void lb_spinner_tick(lb_spinner_t *s) {
  static const char frames[] = "|/-\\";
  if (!s->enabled) return;
  double elapsed = lb__now_seconds() - s->t0;
  char c = frames[s->idx++ & 3u];
  fprintf(s->out, "\r%c %s  %.1fs", c, s->label, elapsed);
  fflush(s->out);
}

static inline void lb_spinner_done(lb_spinner_t *s) {
  if (!s->enabled) return;
  fprintf(s->out, "\r✓ %s\n", s->label);
  fflush(s->out);
}

#else  /* LOADING_BAR_DISABLE */

typedef struct lb_bar_t { int _; } lb_bar_t;
typedef struct lb_spinner_t { int _; } lb_spinner_t;
static inline void lb_bar_init(lb_bar_t *b, size_t total, const char *label, FILE *out) { (void)b;(void)total;(void)label;(void)out; }
static inline void lb_bar_set_width(lb_bar_t *b, int width) { (void)b;(void)width; }
static inline void lb_bar_update(lb_bar_t *b, size_t current) { (void)b;(void)current; }
static inline void lb_bar_done(lb_bar_t *b) { (void)b; }
static inline void lb_spinner_init(lb_spinner_t *s, const char *label, FILE *out) { (void)s;(void)label;(void)out; }
static inline void lb_spinner_tick(lb_spinner_t *s) { (void)s; }
static inline void lb_spinner_done(lb_spinner_t *s) { (void)s; }

#endif /* LOADING_BAR_DISABLE */

#endif /* LOADING_BAR_H */
