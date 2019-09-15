// compile as: gcc -DSELF_TEST -march=i686 -std=c99 -m32 -o csaw csaw.c 
// solution: ./csaw flag{18ea6024aba10c2777703151f5bd1f58} 
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

// flag is: flag{18ea6024aba10c2777703151f5bd1f58}
static uint8_t to_byte(char b) {
    if(b <= '9') {return b - '0';}
    if(b <= 'F') {return b - '7';}
    if(b <= 'f') {return b - 'W';}

    return 0;
}

static uint32_t sum(uint32_t base) {
    if (base == 0) {
        return 0;
    } else if (base == 1) {
        return 1;
    } else {
        return base+sum(base-1);
    }
}

static uint64_t read_bytes(const char *hex, size_t size) {
    uint64_t res = 0;

    for(size_t count = 0; count < size; count++) {
        uint8_t b = to_byte(hex[count]);
        res <<= 4;
        res |= (0xF & b);
    }

    return res;
}

static uint32_t hash(uint64_t val) {
    uint32_t sum = 0;
    for(int i = 0; i < sizeof(val); i++) {
        uint8_t b = val & 0xFF;
        sum += b;
        val >>= 8;
    }

    return sum;
}

int check_flag(const char *flag)
{
    uint32_t flagsize = strlen(flag);
    int ret;

    if( flagsize != 38) {
        return -1;
    }

    if(flag[0] == 'f' &&
       flag[1] == 'l' &&
       flag[2] == 'a' &&
       flag[3] == 'g') 
    {
        ret = '{';
    } 
    else
    {
        return -2;
    }

    if(flag[4] != ret) {
        return -3;
    }

    uint16_t p1 = 0xE607; // 0x18ea
    uint16_t p1_test = (uint16_t)read_bytes(flag+5, 4);
    if((p1 ^ p1_test) == 0xFEED) {
        ret = 0;
    } else {
        ret = -5;
    }

    if(flag[sum(to_byte(flag[6]))+1] != '}') {
        return -4;
    }


    if((flag[9] & 0x0F) == 0x06 && 
       (flag[9] & 0x3C) == 0x34 &&
       (flag[9] & 0xF0) == 0x30) {

        ret = flag[9];

    } else {
        ret = 6;
    }

    ret *= 6;

    uint8_t f10 = to_byte(flag[10]); // 0
    uint8_t f11 = to_byte(flag[11]); // 2
    uint8_t f12 = to_byte(flag[12]); // 4

    ret /= (f10+f11+f12);
    
    if( ret == 6 ) {
        ret = 1;
    } else if(ret == 0x36) {
        ret = 3;
    } else {
        ret = 6;
    }
     
    const char *yab = "yabba daba doo! feed me bad f00d!";
    int cmpres = strncmp(yab+(ret*2)+1, flag+13, ret);

    if(cmpres == 0) {
        ret = 0;
    } else {
        ret = -6;
    }
    
    uint8_t f16 =  flag[16];

    uint64_t f16_c = 0xa4f37d7990a0a7ec; 
    uint64_t f16_fix = f16_c >> f16; // = 0x5279, original val = 0x0c27

    f16_fix ^= 0x5E5E;

    uint64_t f17_20 = read_bytes(flag+17, 4);

    if(f16_fix != f17_20) {
        return -7;
    }

    double vals[] = {0, M_PI/2.0, M_PI, 3.0*M_PI/4.0};
    double sum = 0.0;
    for(int i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
        sum += vals[i]*4.0;
    }
    sum /= M_PI;

    uint32_t f21_29 = read_bytes(flag+21, 8);

    sum /= (double)(9.0);

    // sum = 1.0
    double d1 = f21_29/sum;
    //double d1 = f21_29;
    const char *str = "Q1pW";

    uint32_t back_to_int = (uint32_t)d1;

    const char *str2 = (const char*)&(back_to_int);

    for(int i = 0; i < strlen(str); i++) {
        if( tolower(str[i]) != tolower(str2[i]) ) {
            return -8;
        }
    }

    if( hash(back_to_int) != 0x169) {
        return -9;
    }

    uint64_t f21_37 = read_bytes(flag+21, 16);
    //f5bd 1f58
    uint64_t f29_37 = f21_37 << 32;
    if((hash(f29_37) - 0x7) / 2 != 0x111) {
        ret = 4096;
    } else {
        ret = 8192;
    }

    f29_37 = (f21_37 & 0xFFFFFFFF);
    f29_37 /= 515351531;
    
    ret /= (uint32_t)(f29_37);
    

    if (ret > 512) {
        ret -= 512;
        ret -= hash(f21_37);
        ret += 2;
    }

    return ret+400;
} 

#ifdef SELF_TEST
int main(int argc, const char* argv[]) {

    if(argc < 2) {
        fprintf(stderr, "Must have at least one argument\n");
        return -1;
    }

    int success = check_flag(argv[1]);

    if( success == 0) {
        printf("You win!\n");
    } else {
        printf("You lose: %d\n", success);
    }

    return 0;
}
#endif
