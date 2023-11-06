#ifndef __UTILS_H__
#define	__UTILS_H__


#define AT(index) (auxmem + (index))
#define c_AT(index, type) ((type)(auxmem + (index))) 
#define c_DAT(index, type_in, type_out)  ((type_out)(*((type_in)(auxmem + (index))) + (auxmem)))

#endif