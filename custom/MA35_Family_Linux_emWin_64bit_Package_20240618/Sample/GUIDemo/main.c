/****************************************************************************
 *                                                                          *
 * Copyright (c) 2018 Nuvoton Technology Corp. All rights reserved.         *
 *                                                                          *
 ***************************************************************************/

/****************************************************************************
 *
 * FILENAME
 *     main.c
 *
 * VERSION
 *     1.1
 *
 * DESCRIPTION
 *     To utilize emWin library to demonstrate widgets feature.
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     2019/01/08        Ver 1.1 Updated
 *
 * REMARK
 *     None
 **************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <asm/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/time.h>
#include <pthread.h>  // Thread

#include "GUI.h"
#include "WM.h"

static int vpost_fd;
static struct fb_var_screeninfo fb_vinfo;
static struct fb_fix_screeninfo fb_finfo;

//unsigned char *pVideoBuffer;
void *pVideoBuffer;
int g_xres;
int g_yres;
int g_bits_per_pixel;
int g_line_length;

void APPW_X_FS_Init(void)
{
    /* TODO */
    ;
}
//
// emWin entry point
//
extern void TouchTask(void);

void *MainTask_ISR(void *arg)
{
    printf("Main Task thread\n");
    //
    // Main Loop
    //
    MainTask();
}

void *TouchTask_ISR(void *arg)
{
    printf("Touch Task thread\n");
    TouchTask();
}

void NVT_Sub0(void)
{
    fb_vinfo.yoffset = 0;
    ioctl(vpost_fd, FBIOPAN_DISPLAY, &fb_vinfo);
}

void NVT_Sub1(void)
{
    fb_vinfo.yoffset = fb_vinfo.yres;
    ioctl(vpost_fd, FBIOPAN_DISPLAY, &fb_vinfo);
}

int main()
{
    int ret;
    int i, t = 0;

    FILE *fpVideoImg;

    int uVideoSize;

    pthread_t tid1, tid2;


    vpost_fd = open("/dev/fb0", O_RDWR);
    if (vpost_fd == -1)
    {
        printf("Cannot open fb0!\n");
        return -1;
    }

    if (ioctl(vpost_fd, FBIOGET_VSCREENINFO, &fb_vinfo) < 0)
    {
        perror("ioctl FBIOGET_VSCREENINFO");
        close(vpost_fd);
        return -1;
    }
    //
    // emWin Multiple Buffering
    //
    fb_vinfo.yres_virtual = fb_vinfo.yres * 2;
    fb_vinfo.yoffset = 0;
    if (ioctl(vpost_fd, FBIOPUT_VSCREENINFO, &fb_vinfo))
    {
        perror("ioctl FBIOPUT_VSCREENINFO");
        close(vpost_fd);
        return -1;
    }

    if (ioctl(vpost_fd, FBIOGET_FSCREENINFO, &fb_finfo))
    {
        perror("ioctl FBIOGET_FSCREENINFO");
        close(vpost_fd);
        return -1;
    }

    g_line_length = fb_finfo.line_length;

    uVideoSize = fb_vinfo.xres * fb_vinfo.yres * fb_vinfo.bits_per_pixel / 8 * 2;

    printf("uVideoSize = %d\n", uVideoSize);
    printf("fb_vinfo.xres = %d\n", fb_vinfo.xres);
    printf("fb_vinfo.yres = %d\n", fb_vinfo.yres);
    printf("fb_finfo.line_length = %d\n", fb_finfo.line_length);

    pVideoBuffer = mmap(NULL, uVideoSize, PROT_READ|PROT_WRITE, MAP_SHARED, vpost_fd, 0);

    printf("pVideoBuffer = 0x%08X\n", (unsigned int)((U64)pVideoBuffer));
    if(pVideoBuffer == MAP_FAILED)
    {
        printf("LCD Video Map Failed!\n");
        exit(0);
    }

    g_xres = fb_vinfo.xres;
    g_yres = fb_vinfo.yres;
    g_bits_per_pixel = fb_vinfo.bits_per_pixel;

    pthread_create(&tid1, NULL, MainTask_ISR, (void *)"MainTask");
    pthread_create(&tid2, NULL, TouchTask_ISR, (void *)"TouchTask");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    /* Close LCD */
    munmap(pVideoBuffer, uVideoSize);  //return memory
    close(vpost_fd);
    return 0;
}
