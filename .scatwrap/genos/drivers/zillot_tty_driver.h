<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/drivers/zillot_tty_driver.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H<br>
#define&nbsp;GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H<br>
<br>
#include&nbsp;&lt;genos/drivers/tty_driver.h&gt;<br>
#include&nbsp;&lt;igris/container/ring.h&gt;<br>
#include&nbsp;&lt;zillot/common/uart.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;zillot_tty_driver&nbsp;:&nbsp;public&nbsp;tty_driver<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;zillot::uart&nbsp;*udev&nbsp;=&nbsp;nullptr;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;igris::ring&lt;char&gt;&nbsp;txring&nbsp;=&nbsp;{};<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;zillot_tty_driver()&nbsp;=&nbsp;default;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;zillot_tty_driver(zillot::uart&nbsp;*uart,&nbsp;size_t&nbsp;sz)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;init(uart,&nbsp;sz);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;init(zillot::uart&nbsp;*uart,&nbsp;size_t&nbsp;sz)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;this-&gt;udev&nbsp;=&nbsp;uart;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;txring.resize(sz);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;setup(int32_t&nbsp;baud,&nbsp;char&nbsp;parity,&nbsp;uint8_t&nbsp;databits,&nbsp;uint8_t&nbsp;stopbits);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;static&nbsp;void&nbsp;uartring_irq_handler_tx(void&nbsp;*priv);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;static&nbsp;void&nbsp;uartring_irq_handler_rx(void&nbsp;*priv);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;static&nbsp;void&nbsp;uartring_irq_handler(void&nbsp;*priv,&nbsp;int&nbsp;code);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;size_t&nbsp;transmit(const&nbsp;char&nbsp;*data,&nbsp;size_t&nbsp;size)&nbsp;override;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;~zillot_tty_driver()&nbsp;=&nbsp;default;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;begin();<br>
&nbsp;&nbsp;&nbsp;&nbsp;};<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
