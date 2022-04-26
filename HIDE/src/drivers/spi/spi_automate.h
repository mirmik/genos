#ifndef GENOS_SPI_AUTOMATE_H
#define GENOS_SPI_AUTOMATE_H

namespace genos 
{
	class spi_automate : public spi_device
	{
		struct dlist_head wait_list;

		const uint8_t * _txbuf;
		uint8_t *       _rxbuf;

		int _sendit;
		int _sendlen;

	public:
		genos::spi_driver * drv;

		int exchange(const void *txbuf, void *rxbuf, int len) 
		{
			system_lock();

			while (!dlist_empty(&wait_list)) 
			{
				wait_current_schedee(&wait_list, 0);
			}

			_txbuf = txbuf;
			_rxbuf = rxbuf;
			_sendit = 0;
			_sendlen = len;

			dev->sendbyte(*_txbuf);
			dev->enable_exchange_irq();
			wait_current_schedee(&wait_list, 1);
			
			if (!dlist_empty(&wait_list)) 
				unwait_one(&wait_list); 

			system_unlock();
		}

		void irq_handler() 
		{
			*_rxbuf[_sendit++] = dev->recvbyte();
			
			if (_sendit != _sendlen)
				dev->sendbyte(_txbuf[_sendit]);

			else 
				unwait_one(&wait_list);
		}

		void begin() 
		{
			dev->set_irq_handler(&spi_automate::irq_handler, this);
		}
	};
}

#endif