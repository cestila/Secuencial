/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, Your Name <youremail@domain.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, ACSE & CADIEEL & Your Name <youremail@domain.com
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Short description of this file
 **
 ** Long description of this file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaak.h"
//#include "ejemplo1.h"

/*==================[macros and definitions]=================================*/

#define EDU_CIAA_NXP_LED1 0x08;
#define EDU_CIAA_NXP_LED2 0x10;
#define EDU_CIAA_NXP_LED3 0x20;

#define EDU_CIAA_RGB_VERDE 0x04;
#define EDU_CIAA_RGB_AZUL 0x02;
#define EDU_CIAA_RGB_ROJO 0x01;

/*==================[internal data declaration]==============================*/

static int32_t fd_out;
static int32_t fd_in;
uint8 state=0;
uint8 state_push=0;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

TASK(InitTask) {
   ciaak_start();

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);
   ActivateTask(SwitchTask);
   TerminateTask();
}

TASK(SwitchTask) {
   uint8 inputs;

   ciaaPOSIX_read(fd_in, &inputs, 1);
   SetRelAlarm(ActivatePeriodic,0,100);

   TerminateTask();
}

TASK(PeriodicTask)
{
   uint8 outputs;

   if (state==0) {
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED1;
      ciaaPOSIX_write(fd_out, &outputs, 1);
      state=1;
   }
   else if (state==1) {
      /* blink output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED1;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED2;
      ciaaPOSIX_write(fd_out, &outputs, 1);
      state = 2;
   }
   else if (state==2){
      /* blink output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED2;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED3;
      ciaaPOSIX_write(fd_out, &outputs, 1);
      state=3;
   }
   else if (state==3){
      /* blink output */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED3;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= EDU_CIAA_NXP_LED1;
      ciaaPOSIX_write(fd_out, &outputs, 1);
      state=1;
   }
   TerminateTask();
}



/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
