/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on 
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 *
 * @file        syscall_write.c
 *
 * @brief       This file provides an adaptation for the file write interface.
 *
 * @revision
 * Date         Author          Notes
 * 2020-04-14   OneOS Team      First version.
 ***********************************************************************************************************************
 */
#include <oneos_config.h>
#include <os_device.h>
#ifdef OS_USING_VFS
#include <vfs_posix.h>
#endif
#include <yfuns.h>
#include <libc.h>

#pragma module_name = "?__write"

size_t __write(int handle, const unsigned char *buf, size_t len)
{
#ifdef OS_USING_VFS
    int size;
#endif

    if ((_LLIO_STDOUT == handle) || (_LLIO_STDERR == handle))
    {
#ifndef OS_USING_CONSOLE
        return _LLIO_ERROR;
#else

#ifdef OS_USING_POSIX
        return libc_stdio_write((void *)buf, len);
#else
        os_device_t *console_device;

        console_device = os_console_get_device();
        if (console_device != 0) 
        {
            os_device_write(console_device, 0, buf, len);
        }

        return len;
#endif
#endif
    }

    if (_LLIO_STDIN == handle)
    {
        return _LLIO_ERROR;
    }

#ifndef OS_USING_VFS
    return _LLIO_ERROR;
#else
    size = write(handle, buf, len);
    return size;
#endif
}

