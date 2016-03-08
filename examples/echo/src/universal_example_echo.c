/*

universal_example_echo.c
universal-c

Copyright (c) 2016 Coletiv Studio

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <universal/universal.h>

static void session_runloop(transport_session_t session)
{
    int n = 0;
    unsigned int oldus = 0;
    while (n < 50) 
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if (((unsigned int)tv.tv_usec - oldus) > 50000) 
        {
            oldus = tv.tv_usec; 
            ++n;
            
            if(session)
            {
                transport_session_run(session);
            }
        }
    }
}

void session_open_callback(const void * context)
{   
    printf("universal-example-echo: OPEN");
}

void session_close_callback(const void * context)
{
    printf("universal-example-echo: CLOSE");
}

struct transport_session_handler handler = {
    NULL,
    &session_open_callback,
    &session_close_callback
};

int main() 
{
    transport_session_t session = transport_session_create("universal-example-echo-client", NULL, "wss://localhost", "123456", handler);
    transport_session_open(session);
    session_runloop(session);
    
    // PING
    transport_session_example_ping(session, transport_type_example_create("Hello"));
    
    transport_session_close(session);
    
	return(0);
}