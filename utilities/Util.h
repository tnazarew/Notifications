//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_UTIL_H
#define NOTIFICATOR_UTIL_H


namespace notifications
{
    class Util
    {
        friend class AES;
        friend class RSA;
    public:
        // convert unsigned int to hexademical number in string format
        static void uint32ToHEX(unsigned var, char hvar[8]);
        // convert to unsigned int from hexademical number in string format
        static void hexToUint32(char hvar[8], unsigned& var);
        //
        static void UintToChar(unsigned var, char hvar[4]);

        static void CharToUint(const char hvar[4], unsigned& var);
    };
}


#endif //NOTIFICATOR_UTIL_H
