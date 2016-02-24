#ifndef SLIB_COM_H
#define SLIB_COM_H

/* some commonly used macros */
#define _range(i, min, max)         (i >= min) && (i <= max) ? 1 : 0
#define _rangeDeadband(i, x, db)    (i >= (x-db)) && (i <= (x+db)) ? 1 : 0
#define _rangeInc(i, max)           ((i+1) <= max) ? i+1 : i
#define _rangeDcr(i, min)           ((i-1) >= min) ? i-1 : i
#define _float2int(f)               (int)((f < 0.0f) ? f - 0.5f : f + 0.5f)
#define _float2int16_t(f)           (int16_t)((f < 0.0f) ? f - 0.5f : f + 0.5f)
#define _longLo(x)                  (uint16_t)((unsigned long)x & 0x0000FFFF)
#define _longHi(x)                  (uint16_t)(((unsigned long)x>>16) & 0x0000FFFF)
#define _longLL(x)                  (uint8_t)((unsigned long)x & 0x000000FF)
#define _longLH(x)                  (uint8_t)(((unsigned long)x>>8) & 0x000000FF)
#define _longHL(x)                  (uint8_t)(((unsigned long)x>>16) & 0x000000FF)
#define _longHH(x)                  (uint8_t)(((unsigned long)x>>24) & 0x000000FF)
#define _intLo(x)                   (uint8_t)(x & 0x00FF)
#define _intHi(x)                   (uint8_t)((x>>8) & 0x00FF)
#define _nibLo(x)                   (uint8_t)(x & 0x0F)
#define _nibHi(x)                   (uint8_t)((x>>4)&0x0F)
#define _increment(i, max)          i = (++i) % max
#define nib2char(high, low)         (uint8_t)((((uint8_t)(high & 0x0F))<<4) + ((uint8_t)(low & 0x0F)))
#define char2int(high, low)         (uint16_t)(((uint16_t)high << 8) + (uint16_t)low)
#define char2long(hh, hl, lh, ll)   (unsigned long)(((unsigned long)hh << 24) + ((unsigned long)hl << 16) + ((unsigned long)lh << 8) + ll)

#define int2bcd(i)	(uint8_t)(((i)/10<<4) + (i) % 10)
#define bcd2int(i)	(uint8_t)((((i)>>4) * 10) + ((i) & 0x0F))

#define maskFlagSet(word, mask, b)  b ? (word |= mask) : (word &= ~mask)
#define maskFlagOff(word, mask)     word &= ~mask
#define maskFlagOn(word, mask)      word |= mask
#define maskFlagTest(word, mask)    (word & mask) ? 1 : 0

#define bitFlagSet(word, bit, b)    b ? (word |= (0x01 << bit)) : (word &= ~(0x01 << bit))
#define bitFlagOff(word, bit)       word &= ~(0x01 << bit)
#define bitFlagOn(word, bit)        word |= (0x01 << bit)
#define bitFlagTest(word, bit)      (word & (0x01 << bit)) ? 1 : 0

/****ASM Level Macros****/
/* execute code at this address */
#define _stringize(x)	#x
#define _asm_goto(x)	__asm__("goto " _stringize(x))

/****Error Constants****/
#define SLIB_ERR_NOERROR	0	//no error has occured
#define SLIB_ERR_UNKNOWN	-1	//unknown error
#define SLIB_ERR_NOMEM		-5	//out of memory

#endif  /* SLIB_COM_H */
