//# LimitExec.cc: program to limit the absolute execution time of a program
//#
//#  Copyright (C) 2000,2001,2002
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: LimitExec.cc 9490 2006-10-24 03:15:12Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>


#ifdef USE_NOFORK
int main()
{
  return 1;
}
#else

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static pid_t g_child = 0;
static char* g_progname;
static int g_seconds = 0;

void usage_exit(void)
{
  fprintf(stderr, "Usage: LimitExec <pos. time in seconds> <program> <args>\n");
  exit(EXIT_FAILURE);
}

void killchild(int sig)
{
  /* keep compiler happy */
  sig = sig;
  
  fprintf(stderr, "LimitExec: %s has exceeded time limit (%d s) "
          "and will be killed\n", g_progname, g_seconds);

  if (kill(g_child, SIGKILL) < 0)
  {
    perror("kill");
  }

}

// Translate the exit status of a child process.
// If the child was terminated by a signal, return -1; 
// else return the child's exit status.
int exit_status(int status)
{
  if (WIFSIGNALED(status)) {
    fprintf(stderr, "LimitExec: %s (pid=%d) was terminated "
            "by signal #%d (%s)\n", g_progname, g_child, 
            WTERMSIG(status), strsignal(WTERMSIG(status)));
    return -1;
  }
  else return WEXITSTATUS(status);
}

int main(int argc, char** argv)
{
  int status = 1;

  if (argc < 3) usage_exit();

  g_seconds = atoi(argv[1]);
  if (g_seconds < 0) usage_exit();

  g_progname = argv[2];

  if ((g_child = fork()) < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (0 == g_child)
  {
    /* exec the program with the given parameters */
    if (execvp(argv[2], &argv[2]) < 0)
    {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    /* parent */

    /* install the ALARM signal handler */
    signal(SIGALRM, killchild);

    /* start the timer */
    alarm(g_seconds);

    /* wait for the child to exit
     * either normally or after being killed
     * by the ALARM signal handler.
     */
    (void)wait(&status);
  }

  return exit_status(status);
}

#endif   // USE_NOFORK
