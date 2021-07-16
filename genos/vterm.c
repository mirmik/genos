#include <genos/vterm.h>
#include <genos/signal.h>
#include <igris/defs/vt100.h>

#define CONTTY_LINE_LENGTH 48

void vterm_automate_init(
    struct vterm_automate * vterm,
	char * buffer, unsigned int buffer_size,
	char * hbuffer, unsigned int history_size
)
{
	vterm->state = 0;
	vterm->echo = 1;
	vterm->prefix_string = "$ ";

	vterm->write_callback = NULL;
	vterm->execute_callback = NULL;
	vterm->signal_callback = NULL;

	readline_init(&vterm->rl, buffer, buffer_size);
	readline_history_init(&vterm->rl, hbuffer, history_size);
}

static
void vterm_newline(struct vterm_automate * vterm)
{
	vterm->execute_callback(
	    vterm->execute_privdata,
	    sline_getline(&vterm->rl.line),
	    sline_size(&vterm->rl.line)
	);
	vterm->state = 1;
}


void vterm_automate_newdata(struct vterm_automate * vterm, int16_t input_c)
{
	char c = 0;
	int ret;
	int return_flag = 0;

	while (return_flag == 0)
	{
		switch (vterm->state)
		{
			case 0:
			case 1:
				readline_newline_reset(&vterm->rl);
				vterm->state = 2;
				if (vterm->echo)
				{
					vterm->write_callback(vterm->write_privdata,
					                      vterm->prefix_string, strlen(vterm->prefix_string));
				}

			case 2:
				if (input_c < 0)
				{
					return_flag = 1;
					break;
				}
				else
				{
					c = input_c;
					input_c = -1;
					vterm->state = 3;
				}
				break;

			case 3:
				// CTRL + C
				if (c == 3)
				{
					if (vterm->echo)
						vterm->write_callback(
						    vterm->write_privdata, "^C\r\n", 4);
					vterm->state = 1;

					if (vterm->signal_callback)
						vterm->signal_callback(vterm->signal_privdata, SIGINT);
					break;
				}

				ret = readline_putchar(&vterm->rl, c);

				switch (ret)
				{
					case READLINE_ECHOCHAR:
						{
							if (vterm->echo)
								vterm->write_callback(vterm->write_privdata, &c, 1);

							if (!sline_in_rightpos(&vterm->rl.line))
							{
								char buf[16];

								if (vterm->echo)
								{
									vterm->write_callback(
									    vterm->write_privdata,
									    sline_rightpart(&vterm->rl.line),
									    sline_rightsize(&vterm->rl.line));

									ret = vt100_left(
									          buf,
									          sline_rightsize(&vterm->rl.line));

									vterm->write_callback(
									    vterm->write_privdata,
									    buf,
									    ret);
								}
							}
						}
						break;

					case READLINE_NEWLINE:
						vterm->state = 1;

						if (vterm->echo)
							vterm->write_callback(vterm->write_privdata, "\r\n", 2);

						vterm_newline(vterm);
						return;

					case READLINE_BACKSPACE:
						{
							if (vterm->echo)
							{
								vterm->write_callback(
								    vterm->write_privdata,
								    VT100_LEFT,
								    3);

								vterm->write_callback(
								    vterm->write_privdata,
								    VT100_ERASE_LINE_AFTER_CURSOR,
								    3);
							}
							if (!sline_in_rightpos(&vterm->rl.line))
							{
								char buf[16];

								if (vterm->echo)
								{
									vterm->write_callback(
									    vterm->write_privdata,
									    sline_rightpart(&vterm->rl.line),
									    sline_rightsize(&vterm->rl.line)
									);

									ret = vt100_left(
									          buf,
									          sline_rightsize(&vterm->rl.line));

									vterm->write_callback(
									    vterm->write_privdata,
									    buf,
									    ret);
								}
							}

							break;
						}


					case READLINE_RIGHT:
						if (vterm->echo)
							vterm->write_callback(
							    vterm->write_privdata, VT100_RIGHT, 3);
						break;

					case READLINE_LEFT:
						if (vterm->echo)
							vterm->write_callback(
							    vterm->write_privdata, VT100_LEFT, 3);
						break;

					case READLINE_NOTHING:
						break;

					case READLINE_UPDATELINE:
						{
							char buf[16];

							if (vterm->rl.lastsize)
							{
								if (vterm->echo)
								{
									ret = vt100_left(buf, vterm->rl.lastsize);

									vterm->write_callback(
									    vterm->write_privdata,
									    buf,
									    ret);

									vterm->write_callback(
									    vterm->write_privdata,
									    VT100_ERASE_LINE_AFTER_CURSOR,
									    3);
								}
							}

							if (vterm->rl.line.len)
								if (vterm->echo)
									vterm->write_callback(
									    vterm->write_privdata, 
									    vterm->rl.line.buf, 
									    vterm->rl.line.len);

							break;
						}

					case READLINE_DELETE:
						{
							if (vterm->echo)
							{
								vterm->write_callback(
								    vterm->write_privdata, VT100_ERASE_LINE_AFTER_CURSOR, 3);
							}
							if (!sline_in_rightpos(&vterm->rl.line))
							{
								char buf[16];

								if (vterm->echo)
								{
									vterm->write_callback(
									    vterm->write_privdata,
									    sline_rightpart(&vterm->rl.line),
									    sline_rightsize(&vterm->rl.line));

									ret = vt100_left(buf, sline_rightsize(&vterm->rl.line));

									vterm->write_callback(
									    vterm->write_privdata,
									    buf,
									    ret);
								}
							}

							break;
						}


					default:
						// readline вернул странный код
						// делаем вид, что всё впорядке.
						;
				}

				vterm->state = 2;
				break;

			default:
				// автомат находится в непредусмотренном состоянии
				// сбрасываем состояние и завершаем итерацию.
				vterm->state = 0;			
				return;
		}
	}
}