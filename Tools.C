#include <cstdint>
#include "Tools.h"
#include <string>
#include <iostream>

using namespace std;
/** 
 * builds a 64-bit long out of an array of 8 bytes
 *
 * @param array of 8 bytes
 * @return uint64_t where the low order byte is bytes[0] and
 *           the high order byte is bytes[7]
*/
uint64_t Tools::buildLong(uint8_t bytes[LONGSIZE])
{
   //starting at last array index. work backward, truncating into new uint74_t. 
   uint64_t result = bytes[7];
   result = result << 8 | bytes[6];
   result = result << 8 | bytes[5];
   result = result << 8 | bytes[4];
   result = result << 8 | bytes[3];
   result = result << 8 | bytes[2];
   result = result << 8 | bytes[1];
   result = result << 8 | bytes[0];
   return result;
}

/** 
 * accepts as input an uint64_t and returns the designated byte
 * within the uint64_t; returns 0 if the indicated byte number
 * is out of range 
 *
 * for example, getByte(0x1122334455667788, 7) returns 0x11
 *              getByte(0x1122334455667788, 1) returns 0x77
 *              getByte(0x1122334455667788, 8) returns 0
 *
 * @param uint64_t source that is the source data
 * @param int32_t byteNum that indicates the byte to return (0 through 7)
 * @return 0 if byteNum is out of range
 *           byte 0, 1, .., or 7 of source if byteNum is within range
*/
uint64_t Tools::getByte(uint64_t source, int32_t byteNum)
{
    uint64_t result;
    if (byteNum < 0 || byteNum > 7) {
        return 0;
    } else {
        result = source >> (8 * byteNum);
        result &= 0x00000000000000ff;
    }
    return result;
}

/**
 * accepts as input an uint64_t and returns the bits low through 
 * high of the uint64_t.  bit 0 is the low order bit and bit 63
 * is the high order bit. returns 0 if the low or high bit numbers 
 * are out of range
 *
 * for example, getBits(0x8877665544332211, 0, 7) returns 0x11
 *              getBits(0x8877665544332211, 4, 11) returns 0x21
 *              getBits(0x8877665544332211, 0, 63) returns 0x8877665544332211
 *
 * @param uint64_t source that holds the bits to be grabbed and 
 *        returned
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be returned
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be returned
 * @return an uint64_t that holds a subset of the source bits
 *         that is returned in the low order bits; 0 if low or high 
 *         is out of range
 */
uint64_t Tools::getBits(uint64_t source, int32_t low, int32_t high)
{
    uint64_t result;
    if (low < 0 || high > 63) {
        return 0;
    } else {
        uint64_t result;
        result = source << (63-high);
        result = result >> ((63 - high) + low);
        return result;
        //result = result & (source << high);
        //result = result >> (high - low);
    }
}

/**
 * sets the bits of source in the range specified by the low and high
 * parameters to 1 and returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, setBits(0x1122334455667788, 0, 7) returns 0x11223344556677ff
 *              setBits(0x1122334455667788, 8, 0xf) returns 0x112233445566ff88
 *              setBits(0x1122334455667788, 8, 64) returns 0x1122334455667788
 *                      note: 64 is out of range
 *
 * @param uint64_t source 
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 1
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 1
 * @return an uint64_t that holds the modified source
 */
uint64_t Tools::setBits(uint64_t source, int32_t low, int32_t high)
{
    if (low < 0 || low > 63 || high < 0 || high > 63) {
        return source;
    } else {
        uint64_t answer = source;
        uint64_t mask = 0xffffffffffffffff;
        mask = mask << (63-high);
        mask = mask >> ((63-high) + low);
        mask = mask << low;
        answer = answer | mask;
        return answer;
    }
}

/**
 * sets the bits of source in the range low to high to 0 (clears them) and
 * returns that value. returns source if the low or high
 * bit numbers are out of range
 *
 * for example, setBits(0x1122334455667788, 0, 7) returns 0x1122334455667700
 *              setBits(0x1122334455667788, 8, f) returns 0x1122334455660088
 *
 * @param uint64_t source 
 * @param int32_t low that is the bit number of the lowest numbered
 *        bit to be set to 0
 * @param int32_t high that is the bit number of the highest numbered
 *        bit to be set to 0
 * @return an uint64_t that holds the modified source
 */
uint64_t Tools::clearBits(uint64_t source, int32_t low, int32_t high)
{
    if (low < 0 || low > 63 || high > 63 || high < 0) {
        return source;
    } else {
        uint64_t result = source;
        uint64_t mask = 0xffffffffffffffff;
        mask = mask << (63-high);
        mask = mask >> ((63-high) + low);
        mask = mask << low;
        mask = ~mask;
        result = result & mask;
        return result;
    }
}

/**
 * copies length bits from the source to a destination and returns the
 * modified destination. If low bit number of the source or 
 * dest is out of range or the calculated source or dest high bit 
 * number is out of range, then the unmodified destination is returned.
 *
 * for example,
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 0, 8) 
 *           returns 0x8877665544332288
 *   copyBits(0x1122334455667788, 0x8877665544332211, 0, 8, 8) 
 *           returns 0x8877665544338811
 *
 * @param uint64_t source 
 * @param uint64_t dest 
 * @param int32_t srclow that is the bit number of the lowest numbered
 *        bit of the source to be copied
 * @param int32_t destlow that is the bit number of the lowest numbered
 *        bit of the destination to be modified
 * @param int32_t length that is the number of bits to be copied
 * @return uint64_t that is the modifed dest
 */
uint64_t Tools::copyBits(uint64_t source, uint64_t dest, 
                              int32_t srclow, int32_t dstlow, int32_t length)
{
    uint64_t result = source;
    if (srclow < 0 || srclow > 63 || dstlow < 0 || dstlow > 63) {
        return source;
    } else {
        uint64_t tmp1 = getBits(source,srclow,(srclow + (length -1)));
        uint64_t tmp2 = clearBits(dest,dstlow,(dstlow + (length -1)));
        //cout << "tmp1: " << tmp1 << endl;
        //cout << "tmp2: " << tmp2 << endl;
        result = tmp1 | tmp2;
        //fill in 0xff where the desired bits to be retained are.
        //and the mask with the original for the result.
        return result; 
    }
}

/**
 * sets the bits of source identfied by the byte number to 1 and
 * returns that value. if the byte number is out of range then source
 * is returned unchanged.
 *
 * for example, setByte(0x1122334455667788, 0) returns 0x11223344556677ff
 *              setByte(0x1122334455667788, 1) returns 0x112233445566ff88
 *              setByte(0x1122334455667788, 8) returns 0x1122334455667788
 *
 * @param uint64_t source 
 * @param int32_t byteNum that indicates the number of the byte to be
 *        set to 0xff; the low order byte is byte number 0
 * @return uint64_t that is source with byte byteNum set to 0xff
 */
uint64_t Tools::setByte(uint64_t source, int32_t byteNum)
{
    uint64_t result = source;
    if(byteNum < 0 || byteNum > 7) {
        return source;
    } else {
        uint64_t mask = 0x0000000000000000;
        switch (byteNum) {
            case 0:
                mask = 0x00000000000000ff;
                break;
            case 1:
                mask = 0x000000000000ff00;
                break;
            case 2:
                mask = 0x0000000000ff0000;
                break;
            case 3:
                mask = 0x00000000ff000000;
                break;
            case 4:
                mask = 0x000000ff00000000;
                break;
            case 5:
                mask = 0x0000ff0000000000;
                break;
            case 6:
                mask = 0x00ff000000000000;
                break;
            case 7:
                mask = 0xff00000000000000;
                break;
            default:
                //error.
                break;
        }
        result = result | mask;
        return result;
    }
}

/**
 * assumes source contains a 64 bit two's complement value and
 * returns the sign (1 or 0)
 *
 * for example, sign(0xffffffffffffffff) returns 1
 *              sign(0x0000000000000000) returns 0
 *              sign(0x8000000000000000) returns 1
 *
 * @param uint64_t source
 * @return 1 if source is negative when treated as a two's complement 
 *         value and 0 otherwise
 */
uint8_t Tools::sign(uint64_t source)
{
    //shift by 63. then if 0. 
    uint64_t result = source >> 63;
    return (result == 1) ? 1:0;
}

/**
 * assumes that op1 and op2 contain 64 bit two's complement values
 * and returns true if an overflow would occur if they are summed
 * and false otherwise
 *
 * for example, addOverflow(0x8000000000000000, 0x8000000000000000) returns 1
 *              addOverflow(0x7fffffffffffffff, 0x7fffffffffffffff) returns 1
 *              addOverflow(0x8000000000000000, 0x7fffffffffffffff) returns 0
 *
 * @param uint64_t op1 that is one of the operands of the addition
 * @param uint64_t op2 that is the other operand of the addition
 * @return true if op1 + op2 would result in an overflow assuming that op1
 *         and op2 contain 64-bit two's complement values
 */
bool Tools::addOverflow(uint64_t op1, uint64_t op2)
{
    //testing for overflow in 2's comp... the rules are:
    //if both # positive and result is neg. then overflow
    //if both # negative and result is pos. then overflow. 
    //else no overflow. 
    uint64_t result = op1+op2;

    if (sign(op1) != sign(op2)){
       return false; 
    } else {
       if (sign(op1) == sign(result)) {
            return false;
       } else {
            return true;
       }
    }
}

