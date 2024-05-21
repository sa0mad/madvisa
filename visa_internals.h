#ifndef __VISA_INTERNALS_H
#define __VISA_INTERNALS_H

ViSession vi_next_();

ViStatus viGetFakeSession(ViSession * vi);
ViStatus vi_flush_pool();
ViStatus viCleanup();

#endif // __VISA_INTERNALS_H

