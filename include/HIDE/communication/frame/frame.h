#ifndef GENOS_FRAME_FORMER_H
#define GENOS_FRAME_FORMER_H

#include <string.h>

#define SEQSIZE 3
#define CHECKSUM_SIZE 2

#define SEQ_FIRST_SYMBOL 0xAA
#define SEQ_SECOND_SYMBOL 0xBB

#define STRT_SEQ_THIRD_SYMBOL 0xCC
#define FINI_SEQ_THIRD_SYMBOL 0xDD
#define STUB_SEQ_THIRD_SYMBOL 0xEE

#define STRT_SEQ_STUB_SYMBOL 0xAF
#define FINI_SEQ_STUB_SYMBOL 0xF8
#define STUB_SEQ_STUB_SYMBOL 0xB1

#define PARSE_INCORRECT_START_SEQUENCE -1
#define PARSE_INCORRECT_STUB_SEQUENCE -2
#define PARSE_INCORRECT_CHECKSUMM -3
#define PARSE_FAST_RESTART -4
#define PARSE_END_OF_BUFFER -5
#define PARSE_CORRECT_RECEIVE 20

#define PARSE_WRONG_PACKAGE_LENGTH -6

/*
  Name  : CRC-16 CCITT
  Poly  : 0x8408
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x6F91 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
uint16_t  crc_ccitt_update (uint16_t crc, uint8_t  data){
	uint16_t  t;
	data ^= crc&255;
	data ^= data << 4;
	t = (((uint16_t )data << 8) | ((crc>>8)&255));
	t^=(uint16_t )(data >> 4);
	t^= ((uint16_t )data << 3);
	return t;
}

static int frame_form(const char* str, uint16_t sz, char* outbuf, size_t maxlen) {
	__label__ stdcnt;
	uint8_t tmp;
	uint16_t crc = 0;
	
	int summary_size = sz + SEQSIZE * 2;
	uint8_t* cursor;
	int cmpsymb = 0;

	if (summary_size > maxlen) return -1;

	cursor = (uint8_t*)outbuf;
	*cursor++ = SEQ_FIRST_SYMBOL;
	*cursor++ = SEQ_SECOND_SYMBOL;
	*cursor++ = STRT_SEQ_THIRD_SYMBOL;

	*cursor++ = (sz >> 8) & 0xFF;
	*cursor++ = sz & 0xFF;

	const uint8_t * ptr = (const uint8_t*)str;
	const uint8_t * end = ptr + sz;

	for(; ptr != end; ++ptr) {
		tmp = *ptr;

		crc = crc_ccitt_update(crc, tmp);

		switch (cmpsymb) {
			case 0 :
				if (tmp == SEQ_FIRST_SYMBOL) cmpsymb = 1;
				break; 
			case 1 :
				if (tmp == SEQ_SECOND_SYMBOL) cmpsymb = 2; else cmpsymb = 0;
				break; 
			case 2 :
				cmpsymb = 0;
				switch(tmp) {
					case STRT_SEQ_THIRD_SYMBOL: tmp = STRT_SEQ_STUB_SYMBOL; break;
					case FINI_SEQ_THIRD_SYMBOL: tmp = FINI_SEQ_STUB_SYMBOL; break;
					case STUB_SEQ_THIRD_SYMBOL: tmp = STUB_SEQ_STUB_SYMBOL; break;
					default: goto stdcnt;
				}
				if (++summary_size > maxlen) return -1; 
				*cursor++ = STUB_SEQ_THIRD_SYMBOL;
				break;
		}
		stdcnt:
		*cursor++ = tmp;
	}

	*cursor++ = (crc >> 8) & 0xFF;
	*cursor++ = crc & 0xFF;

	*cursor++ = SEQ_FIRST_SYMBOL;
	*cursor++ = SEQ_SECOND_SYMBOL;
	*cursor++ = FINI_SEQ_THIRD_SYMBOL;

	return cursor - (uint8_t*)outbuf;
}


struct frame_parse_s {
	char* outbuf;
	char* cursor;
	size_t max;

	int cmpsymb;
	size_t pkgsize;
	size_t rcvsize;

	uint8_t state;
};

void __frame_parse_reinit(struct frame_parse_s* fprs) {
	fprs->cursor = fprs->outbuf;
	fprs->state = 0;
	fprs->cmpsymb = 0;
	fprs->pkgsize = 0;
	fprs->rcvsize = 0;
}

void frame_parse_init(struct frame_parse_s* fprs, char* outbuf, size_t max) {
	fprs->outbuf = outbuf;
	fprs->max = max;
	__frame_parse_reinit(fprs);
}

int frame_parse_check_correct(struct frame_parse_s* fprs) {
	if (fprs->pkgsize != fprs->rcvsize - CHECKSUM_SIZE) return PARSE_WRONG_PACKAGE_LENGTH;

	uint16_t crc = 0;
	for (int i = 0; i < fprs->pkgsize; i++) {
		crc = crc_ccitt_update(crc, *(fprs->outbuf + i));		
	}

	uint8_t crchi = *(fprs->outbuf + fprs->pkgsize);
	uint8_t crclo = *(fprs->outbuf + fprs->pkgsize + 1);
	uint16_t crc2 = (crchi << 8) + crclo;

	if (crc != crc2) return PARSE_INCORRECT_CHECKSUMM;

	return PARSE_CORRECT_RECEIVE;
}

int frame_parse_char(struct frame_parse_s* fprs, uint8_t c) {
	switch (fprs->state) {
		case 0:
			if (c == SEQ_FIRST_SYMBOL) { fprs->state = 1; return 0; } else { fprs->state = 0; return PARSE_INCORRECT_START_SEQUENCE; };
		case 1:
			if (c == SEQ_SECOND_SYMBOL) { fprs->state = 2; return 0; } else { fprs->state = (c == SEQ_FIRST_SYMBOL) ? 1 : 0; return PARSE_INCORRECT_START_SEQUENCE; };
		case 2: 
			if (c == STRT_SEQ_THIRD_SYMBOL) { fprs->state = 3; return 0; } else { fprs->state = (c == SEQ_FIRST_SYMBOL) ? 1 : 0; return PARSE_INCORRECT_START_SEQUENCE; };	
		case 3: 
			fprs->pkgsize = ((uint16_t)c) << 8; 
			fprs->state = 4; 
			break;
		case 4: 
			fprs->pkgsize += c; 
			fprs->state = 5; 
			break;
		case 5:
			switch (fprs->cmpsymb) {
				case 0: if (c == SEQ_FIRST_SYMBOL) { fprs->cmpsymb = 1; break; } else { fprs->cmpsymb = 0; break; };
				case 1: if (c == SEQ_SECOND_SYMBOL) { fprs->cmpsymb = 2; break; } else { fprs->cmpsymb = (c == SEQ_FIRST_SYMBOL) ? 1 : 0; break; };
				case 2: 
					switch (c) {
						case STRT_SEQ_THIRD_SYMBOL : __frame_parse_reinit(fprs); fprs->state = 1; return PARSE_FAST_RESTART;
						case FINI_SEQ_THIRD_SYMBOL : fprs->rcvsize -= SEQSIZE - 1; return frame_parse_check_correct(fprs);
						case STUB_SEQ_THIRD_SYMBOL : fprs->state = 6; return 0; 
					}
			}
			if (++fprs->rcvsize > fprs->max) { __frame_parse_reinit(fprs); return PARSE_END_OF_BUFFER; }
			*fprs->cursor++ = c;
			return 0;
		case 6:
			if (++fprs->rcvsize > fprs->max) { __frame_parse_reinit(fprs); return PARSE_END_OF_BUFFER; }
			switch (c) {
				case STRT_SEQ_STUB_SYMBOL : *fprs->cursor++ = STRT_SEQ_THIRD_SYMBOL; break;
				case FINI_SEQ_STUB_SYMBOL : *fprs->cursor++ = FINI_SEQ_THIRD_SYMBOL; break;
				case STUB_SEQ_STUB_SYMBOL : *fprs->cursor++ = STUB_SEQ_THIRD_SYMBOL; break;
				default : __frame_parse_reinit(fprs); return PARSE_INCORRECT_STUB_SEQUENCE; break;
			}
			fprs->state = 5;
			return 0;
	}
}

#endif