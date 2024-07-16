#include <nos/trent/trent.h>

void set_buffered_channels(const nos::trent &tr);
void set_buffered_channels_timelapse(double lapse);
void buffered_channels_clean();
void listenerThread_BufferedChannels_Func(void *);

void start_buffered_channels_automate();
void stop_buffered_channels_automate();
void buffered_channels_clean();
