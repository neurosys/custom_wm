/*
 * Copyright (C) 2014,2015 levi0x0 with enhancements by ProgrammerNerd
 * 
 * barM (bar_monitor or BarMonitor) is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 *  This is a new version of bar monitor, even less lines of code more effective.
 *
 *  Read main() to configure your new status Bar.
 *
 *  compile: gcc -o barM barM.c -O2 -s -lX11
 *  
 *  mv barM /usr/local/bin/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

/*
 *  Put this in your .xinitrc file: 
 *
 *  barM&
 *  
 */

#define VERSION "0.12"
#define TIME_FORMAT "[%Y-%m-%d] (%H:%M:%S)"
#define MAXSTR  1024

#define SLEEP_UNIT_S 1
#define BATTERY_REFRESH_TIME 60 * SLEEP_UNIT_S
int get_acpi_output(char* buf, int size);
void extract_acpi_info(char* buf, char* out);

static const char * date(void);
//static const char * ram(void);
static const char * battery(void);

static void XSetRoot(const char *name);
/*Append here your functions.*/
static const char*(*const functab[])(void)={
        battery,
        date
};

int main(void){
        char status[MAXSTR];
        /* It is foolish to repeatedly update uname. */
        int ret = 0;
        {
            struct utsname u;
            if(uname(&u)){
                perror("uname failed");
                return 1;
            }
            /* ret=snprintf(status,sizeof(status),"(%s %s %s) ",u.sysname,u.nodename,u.release); */
        }
        char*off=status+ret;
        if(off>=(status+MAXSTR)){
                XSetRoot(status);
                return 1;/*This should not happen*/
        }
        for(;;){
                int left=sizeof(status)-ret;
                unsigned int i;
                char*sta=off;
                for(i = 0; i<sizeof(functab)/sizeof(functab[0]); ++i ) {
                        int ret=snprintf(sta,left,"%s ",functab[i]());
                        sta+=ret;
                        left-=ret;
                        if(sta>=(status+MAXSTR))/*When snprintf has to resort to truncating a string it will return the length as if it were not truncated.*/
                                break;
                }
                XSetRoot(status);
                sleep(SLEEP_UNIT_S);
        }
        return 0;
}

/* Returns the date*/
static const char * date(void){
        static char date[MAXSTR];
        time_t now = time(0);

        strftime(date, MAXSTR, TIME_FORMAT, localtime(&now));
        return date;
}
/* Returns a string that contains the amount of free and available ram in megabytes*/
/*
static const char * ram(void){
        static char ram[MAXSTR];
        struct sysinfo s;
        sysinfo(&s);
        snprintf(ram,sizeof(ram),"%.1fM,%.1fM",((double)(s.totalram-s.freeram))/1048576.,((double)s.totalram)/1048576.);
        return ram;
}
*/

static void XSetRoot(const char *name){
        Display *display;

        if (( display = XOpenDisplay(0x0)) == NULL ) {
                fprintf(stderr, "[barM] cannot open display!\n");
                exit(1);
        }

        XStoreName(display, DefaultRootWindow(display), name);
        XSync(display, 0);

        XCloseDisplay(display);
}

static const char* battery()
{
    static int count = 0;
    static char bat[128];
    if (count != 0)
    {
        count++;
        count = (count % BATTERY_REFRESH_TIME);
        return bat;
    }
    count++;
    char buf[256];
    char out[16];
    get_acpi_output(buf, 256);
    extract_acpi_info(buf, out);
    snprintf(bat, 128, " [Bat %s] ", out);
    return bat;
}

void extract_acpi_info(char* buf, char* out) {

    char* start_match = strstr(buf, "Battery ");

    if (start_match == NULL || start_match != buf)
    {
        // No match or match in some wierd place
        strcpy(out, "Can't parse acpi output (1)");
        return;
    }

    char* cursor = index(buf, ':');
    cursor += 2; // by doing this we should position on the actual action
                      // Full / Charging / Discharging

    //printf("(2) cursor = '%s'\n", cursor);
    if (strncmp(cursor, "Full, ", 6) == 0)
    {
        out[0] = 'F';
        cursor += 6;
    }
    else if (strncmp(cursor, "Discharging, ", 13) == 0)
    {
        out[0] = 'D';
        cursor += 13;
    }
    else if (strncmp(cursor, "Charging, ", 10) == 0)
    {
        out[0] = 'C';
        cursor += 10;
    }
    else
    {
        strcpy(out, "Can't parse acpi output (2)");
        return;
    }
    out[1] = ' ';

    //printf("(3) cursor = '%s'\n", cursor);

    int i = 0;
    for (i = 0; i < 4 && cursor[i] != '%'; i++)
    {
        out[2 + i] = cursor[i];
    }
    //printf("(4) i = %d cursor = '%s'\n", i, cursor + i);
    i++; // now out + i is on the position for '%'
    out[i + 1] = '%'; // set the '%'
    i += 2; // take the position for the '\0';
    out[i] = '\0';

    if (out[0] == 'F')
    {
        return;
    }
    else if (strstr(out, "100%") != NULL)
    {
        return;
    }

    cursor += i;
    //printf("(5) i = %d cursor = '%s'\n", i, cursor);

    if (cursor[2] == ':' && cursor[5] == ':')
    {
        strncat(out, " ", 1);
        strncat(out, cursor, 8);
        //out[9] = '\0';
    }
    else
    { 
        // Charging at slow rate
        strncat(out, " ", 1);
        strncat(out, "Never", 5);
    }

    return ;
}

int get_acpi_output(char* buf, int size) {
    FILE* f = popen("acpi", "r");
    int output_length = fread(buf, size, 1, f);
    pclose(f);
    return output_length;
}
