#include <iostream>
#include "VirtualMachine.h"

ExecutionException::ExecutionException(std::string reason) : reason(reason) {}
std::string ExecutionException::what() const {
    return "CLEOD Virtual Machine encountered an error: " + reason;
}

VirtualMachine::VirtualMachine(Bytecode &code) : code(code) {}
void VirtualMachine::execute() {
    while(!code.atEnd()) {
        switch(code.nextOpcode()) {
            case Opcode::LITERAL:
                pushNextLiteral(); break;
            case Opcode::PRINT:
                print(); break;
            case Opcode::ADD:
                add(); break;
            case Opcode::SUBTRACT:
                subtract(); break;
            case Opcode::MULTIPLY:
                multiply(); break;
            case Opcode::DIVIDE:
                divide(); break;
            case Opcode::LT:
                lt(); break;
            case Opcode::BNE:
                bne(); break;
            default:
                break;
        }
    }

    //  Do GC (very trivial atm) to prevent memory leaks
    gc.cleanAll();
}

void VirtualMachine::pop(Data &d) {
    d = stack.top();
    stack.pop();
}
Data VirtualMachine::pop() {
    Data d;
    pop(d);
    return d;
}

void VirtualMachine::pushNextLiteral() {
    DataType dt = code.nextDataType();
    StringObject *so;
    VarObject *vo;
    switch(dt) {
        case DataType::DOUBLE:
            stack.push(Data(code.nextDouble()));
            break;
        case DataType::STRING:
            so = new StringObject(code.nextString());
            gc.add(so);
            stack.push(Data(so));
            break;
        case DataType::BOOL:
            stack.push(Data(code.nextBool()));
            break;
        case DataType::VAR:
            //  assumes this was preceded by a push literal of the assigned data type
            vo = new VarObject(new Data(pop()));
            gc.add(vo);
            varIDHashTable[code.nextString()] = vo;
            stack.push(Data(vo));
            break;
        default:
            break;
    }
}

void VirtualMachine::print() {
    Data top = pop();
    StringObject *so;
    switch(top.type) {
        case DataType::DOUBLE:
            out << top.data.d << std::endl; break;
        case DataType::STRING:
            so = dynamic_cast<StringObject *>(top.data.o);
            out << so->s << std::endl;
            break;
        case DataType::BOOL:
            if(top.data.b)
                out << "true" << std::endl;
            else
                out << "false" << std::endl;
            break;
        case DataType::VAR:
            //  dereference data contained by variable and print
        default:
            break;
    }
}

void VirtualMachine::add() {
    //  pop will get us the data in reverse order from how it was pushed - this doesn't matter for add, will for others.
    Data d2 = pop();
    Data d1 = pop();
    if(d1.type == DataType::DOUBLE && d2.type == DataType::DOUBLE) {
        double result = d1.data.d + d2.data.d;
        stack.push(Data(result));
    }
}
void VirtualMachine::subtract() {
    Data d2 = pop();
    Data d1 = pop();
    if(d1.type == DataType::DOUBLE && d2.type == DataType::DOUBLE) {
        double result = d1.data.d - d2.data.d;
        stack.push(Data(result));
    }
}
void VirtualMachine::multiply() {
    Data d2 = pop();
    Data d1 = pop();
    if(d1.type == DataType::DOUBLE && d2.type == DataType::DOUBLE) {
        double result = d1.data.d * d2.data.d;
        stack.push(Data(result));
    }
}
void VirtualMachine::divide() {
    Data d2 = pop();
    Data d1 = pop();
    if(d1.type == DataType::DOUBLE && d2.type == DataType::DOUBLE) {
        double result = d1.data.d / d2.data.d;
        stack.push(Data(result));
    }
}
void VirtualMachine::lt(){
    Data d2 = pop();
    Data d1 = pop();
    if (d1.type == DataType::DOUBLE && d2.type == DataType::DOUBLE){
        bool rslt = (d1.data.d < d2.data.d);
        stack.push(Data(rslt));
    }
}
void VirtualMachine::bne(){
    // fill
    cluint jumpLoc = code.nextUint();
    Data d1 = pop();
    if (d1.type == DataType::BOOL){
        if (!d1.data.b){
            code.setHead(jumpLoc);
        }
    }
}
