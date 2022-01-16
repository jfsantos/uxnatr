#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "uxn.h"

/* #include "devices/system.h"
#include "devices/screen.h"
#include "devices/audio.h"
#include "devices/file.h"
#include "devices/controller.h"
#include "devices/mouse.h"
#include "devices/datetime.h"
*/

/*
Copyright (c) 2021 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

#define WIDTH 64 * 8
#define HEIGHT 40 * 8
#define PAD 4
#define BENCH 0

/* devices */

static Device *devsystem, *devscreen, *devmouse, *devctrl, *devaudio0, *devconsole;
static Uint32 stdin_event, audio0_event;

static int
clamp(int val, int min, int max)
{
	return (val >= min) ? (val <= max) ? val : max : min;
}

static int
error(char *msg, const char *err)
{
	fprintf(stderr, "%s: %s\n", msg, err);
	return 0;
}

static void
redraw(void)
{
}

static int
init(void)
{
	return 0;
}

void
system_deo_special(Device *d, Uint8 port)
{
	if(port > 0x7 && port < 0xe)
		screen_palette(&uxn_screen, &d->dat[0x8]);
}

static void
console_deo(Device *d, Uint8 port)
{
	if(port > 0x7)
		write(port - 0x7, (char *)&d->dat[port], 1);
}

static Uint8
audio_dei(Device *d, Uint8 port)
{
}

static void
audio_deo(Device *d, Uint8 port)
{
}

static Uint8
nil_dei(Device *d, Uint8 port)
{
	return d->dat[port];
}

static void
nil_deo(Device *d, Uint8 port)
{
	(void)d;
	(void)port;
}

/* Boot */

static int
load(Uxn *u, char *rom)
{
	int r;
	r = f->read(f, u->ram + PAGE_PROGRAM, 1, 0x10000 - PAGE_PROGRAM);
	f->close(f);
	if(r < 1) return 0;
	fprintf(stderr, "Loaded %s\n", rom);
	return 1;
}

static Uint8 *shadow, *memory;

static int
start(Uxn *u, char *rom)
{
	memory = (Uint8 *)calloc(0x10000, sizeof(Uint8));
	shadow = (Uint8 *)calloc(0x10000, sizeof(Uint8));

	if(!uxn_boot(&supervisor, shadow, shadow + VISOR_DEV, (Stack *)(shadow + VISOR_WST), (Stack *)(shadow + VISOR_RST)))
		return error("Boot", "Failed to start uxn.");
	if(!uxn_boot(u, memory, shadow + PAGE_DEV, (Stack *)(shadow + PAGE_WST), (Stack *)(shadow + PAGE_RST)))
		return error("Boot", "Failed to start uxn.");
	if(!load(&supervisor, "supervisor.rom"))
		error("Supervisor", "No debugger found.");
	if(!load(u, rom))
		return error("Boot", "Failed to load rom.");

	/* system   */ devsystem = uxn_port(u, 0x0, system_dei, system_deo);
	/* console  */ devconsole = uxn_port(u, 0x1, nil_dei, console_deo);
	/* screen   */ devscreen = uxn_port(u, 0x2, screen_dei, screen_deo);
	/* audio0   */ devaudio0 = uxn_port(u, 0x3, audio_dei, audio_deo);
	/* audio1   */ uxn_port(u, 0x4, audio_dei, audio_deo);
	/* audio2   */ uxn_port(u, 0x5, audio_dei, audio_deo);
	/* audio3   */ uxn_port(u, 0x6, audio_dei, audio_deo);
	/* unused   */ uxn_port(u, 0x7, nil_dei, nil_deo);
	/* control  */ devctrl = uxn_port(u, 0x8, nil_dei, nil_deo);
	/* mouse    */ devmouse = uxn_port(u, 0x9, nil_dei, nil_deo);
	/* file     */ uxn_port(u, 0xa, nil_dei, file_deo);
	/* datetime */ uxn_port(u, 0xb, datetime_dei, nil_deo);
	/* unused   */ uxn_port(u, 0xc, nil_dei, nil_deo);
	/* unused   */ uxn_port(u, 0xd, nil_dei, nil_deo);
	/* unused   */ uxn_port(u, 0xe, nil_dei, nil_deo);
	/* unused   */ uxn_port(u, 0xf, nil_dei, nil_deo);

	/* Supervisor */
	uxn_port(&supervisor, 0x0, system_dei, system_deo);
	uxn_port(&supervisor, 0x1, nil_dei, console_deo);
	uxn_port(&supervisor, 0x2, screen_dei, screen_deo);
	uxn_port(&supervisor, 0x8, nil_dei, nil_deo);

	uxn_eval(&supervisor, PAGE_PROGRAM);

	if(!uxn_eval(u, PAGE_PROGRAM))
		return error("Boot", "Failed to start rom.");

	return 1;
}

static void
restart(Uxn *u)
{
	start(u, "boot.rom");
}

static int
console_input(Uxn *u, char c)
{
	devconsole->dat[0x2] = c;
	return uxn_eval(u, GETVECTOR(devconsole));
}

static int
run(Uxn *u)
{
	while(!devsystem->dat[0xf]) {
		if(devsystem->dat[0xe])
			uxn_eval(&supervisor, GETVECTOR(&supervisor.dev[2]));
		uxn_eval(u, GETVECTOR(devscreen));
	}
	return error("Run", "Ended.");
}

int
main(int argc, char **argv)
{
	Uxn u;
	int i, loaded = 0;

	if(!init())
		return error("Init", "Failed to initialize emulator.");
	if(!set_size(WIDTH, HEIGHT, 0))
		return error("Window", "Failed to set window size.");
	if(!loaded && !start(&u, "boot.rom"))
		return error("usage", "uxnemu [-s scale] file.rom");
	run(&u);
	return 0;
}
