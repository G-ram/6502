#include "headers/reg.h"
#include "headers/log.h"

std::ostream& operator<<(std::ostream& out, const Reg& r){
    out << "A: " << hexify(r.A.uw) << std::endl;
    out << "X: " << hexify(r.X.uw) << std::endl;
    out << "Y: " << hexify(r.Y.uw) << std::endl;
    out << "P: " << binify(r.P.uw) << std::endl;
    out << "S: " << hexify(r.S.uw) << std::endl;
    out << "PC: " << hexify(r.PC.udw);
    return out;
}
