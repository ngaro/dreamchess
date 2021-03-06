/*  DreamChess
**
**  DreamChess is the legal property of its developers, whose names are too
**  numerous to list here. Please refer to the AUTHORS.txt file distributed
**  with this source distribution.
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstring>
#include <windows.h>

#include "msgbuf.h"
#include "pipe_win32.h"

#define BUF_LEN 1024

/* Static buffer. */
static char buf[BUF_LEN];

static HANDLE h_in, h_out;

/* Console mode flag */
static int console_mode;

void pipe_win32_init(HANDLE in, HANDLE out, int console)
{
	h_in = in;
	h_out = out;
	console_mode = console;
}

void pipe_win32_exit(void)
{
	msgbuf_exit();
}

void pipe_win32_send(const char *m)
{
	DWORD written;

	if (!WriteFile(h_out, m, (DWORD)std::strlen(m), &written, nullptr) || (written < std::strlen(m))) {
		std::fprintf(stderr, "%s, L%d: Error writing to pipe.\n", __FILE__, __LINE__);
		exit(1);
	}
}

char *pipe_win32_poll(int *error)
{
	*error = 0;

	/* Repeat until no more data is available, or a full message has been
	** received.
	*/
	while (1) {
		DWORD bytes;
		char *msg;
		std::size_t len;

		if ((msg = msgbuf_process(buf)))
			return msg;

		len = std::strlen(buf);

		/* Check whether data is available. */
		if (console_mode) {
			if (!GetNumberOfConsoleInputEvents(h_in, &bytes)) {
				/* Error reading console input. */
				std::fprintf(stderr, "%s, L%d: Error reading console input.\n", __FILE__, __LINE__);
				*error = 1;
				return nullptr;
			}
		} else if (!PeekNamedPipe(h_in, nullptr, 0, nullptr, &bytes, nullptr)) {
			/* Error reading pipe. */
			std::fprintf(stderr, "%s, L%d: Broken pipe.\n", __FILE__, __LINE__);
			*error = 1;
			return nullptr;
		}

		if (bytes > 0) {
			/* Read data. */
			if (!ReadFile(h_in, buf + len, BUF_LEN - (DWORD)len - 1, &bytes, nullptr)) {
				/* Error reading pipe. */
				std::fprintf(stderr, "%s, L%d: Broken pipe.\n", __FILE__, __LINE__);
				*error = 1;
				return nullptr;
			}

			if (!console_mode && bytes == 0) {
				/* Received EOF. */
				std::fprintf(stderr, "%s, L%d: Broken pipe.\n", __FILE__, __LINE__);
				*error = 1;
				return nullptr;
			}

			buf[len + bytes] = '\0';
		} else
			/* No data available. */
			break;
	}
	return nullptr;
}
