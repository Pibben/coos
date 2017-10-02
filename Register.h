#include <cstdint>

#include "utils.h"

class Register {
public:
    Register(uintptr_t address) : mAddress((uint32_t*)address) {
    }
    Register(const Register&) = delete;
    Register& operator=(const Register&) = delete;

    void write(uint32_t value) {
        *mAddress = value;
    }
    void write(uint32_t value, uint_fast8_t shift) {
        *mAddress |= (value << shift);
    }
    void writeOnly(uint32_t value, uint_fast8_t shift) {
        *mAddress = (value << shift);
    }
    uint32_t read() const {
        return *mAddress;
    }
    uint32_t read(uint_fast8_t shift, uint_fast8_t numBits) const {
        return (*mAddress >> shift) & ((1 << numBits)-1);
    }
    bool get(uint_fast8_t bit) {
        return *mAddress & (1 << bit);
    }
    template<class... Ts>
    void set(uint_fast8_t bit, Ts... bits) {
        *mAddress |= Util::valueBuilder<Ts...>(0, bit, bits...);
    }
    void set() {
        *mAddress = 0xffffffff;
    }
    template<class... Ts>
    void setOnly(uint_fast8_t bit, Ts... bits) {
        *mAddress = Util::valueBuilder<Ts...>(0, bit, bits...);
    }
    template<class...Ts>
    void clear(uint_fast8_t bit, Ts... bits) {
        *mAddress &= ~Util::valueBuilder<Ts...>(0, bit, bits...);
    }
    void clear() {
        *mAddress = 0;
    }
    operator uint32_t() const {
        return read();
    }
    uintptr_t getAddress() {
        return (uintptr_t)mAddress;
    }
private:
    volatile uint32_t* mAddress;
};
