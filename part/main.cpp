#include "systemc.h"
#include "cstdlib"
#include "ctime"

class writeI : virtual public sc_interface
{
public:
    virtual void write(sc_uint<8> data) = 0;
};

class fctI : virtual public sc_interface
{
public:
    virtual void fct() = 0;
};

class trafgen : public sc_module
{
public:
    sc_port<writeI> out_port;
    sc_uint<8> x;
    sc_event send;
    int runs;

    SC_HAS_PROCESS(trafgen);

    trafgen(sc_module_name mn, int param = 1) : sc_module(mn), runs(param)
    {
        srand(time(0));
        SC_THREAD(gen_event);
        sensitive << send;
    }

    void gen_event()
    {
        for (int r = 0; r < runs; r++)
        {
            for (int i = 0; i < 19; i++)
            {
                x = (sc_uint<8>)rand() % 255;
                send_byte();
            }
            x = 255;
            send_byte();
        }
    }

    void send_byte()
    {
        out_port->write(x);
        send.notify(5, SC_NS);
        wait();
//		cout << "send " << x << '\n';
    }
};

class receiver : public sc_module, public writeI, public fctI
{
public:
    sc_fifo<sc_uint<8> > write_buf;
    sc_uint<8> tmp_byte;
    sc_event eop;
    sc_port<sc_fifo_out_if<sc_uint<8> > > out_port;
    bool port_is_ready;

    SC_HAS_PROCESS(receiver);

    receiver(sc_module_name mn) : sc_module(mn), write_buf(40)
    {
        port_is_ready = false;
        SC_METHOD(printer);
        sensitive << eop;
    }

    virtual void write(sc_uint<8> data)
    {
        if (data == 255)
            eop.notify(0, SC_MS);
        else
            write_buf.write(data);
    }

    virtual void fct()
    {
        port_is_ready = true;
        cout << this->basename() << " received fct at " << sc_time_stamp() << "\n";
    }

    void printer()
    {
        while (write_buf.nb_read(tmp_byte))
        {          
            if (port_is_ready)
            {
                out_port->write(tmp_byte);
                cout << this->basename() << " send " << tmp_byte << " at " <<sc_time_stamp() << "\n";
                port_is_ready = false;
            }
        }
        cout << '\n';
    }
};

class router : public sc_module
{
public:
    sc_port<fctI> fct_port1, fct_port2;
    sc_port<sc_fifo_out_if<sc_uint<8> > > in1, in2;
    sc_fifo<sc_uint<8> > buf1, buf2;
    SC_HAS_PROCESS(router);

    router(sc_module_name mn): sc_module(mn), buf1(8), buf2(8)
    {
        in1.bind(buf1);
        in2(buf2);
        SC_METHOD(read_port2);
        SC_THREAD(read_data_from_in1)
    }

    void read_data_from_in1()
    {
        while (true)
        {
            fct_port1->fct();
            sc_uint<8> tmp_byte = buf1.read();
            cout << this->basename() << " received " << tmp_byte << " at " <<  sc_time_stamp() << "\n";
        }
    }


    void read_port2()
    {
        fct_port2->fct();
    }

};

int sc_main(int argc, char* argv[])
{
    int k = 1;
    if (argc == 2)
        k = atoi(argv[1]);

    trafgen t1("trafgen1", k);
    receiver r1("receiver1");
    t1.out_port(r1);

    trafgen t2("trafgen2", k);
    receiver r2("receiver2");
    t2.out_port(r2);

    router routerE("router");
    routerE.fct_port1(r1);
    routerE.fct_port2(r2);

    r1.out_port(routerE.in1);
    r2.out_port(routerE.in2);
    sc_start(2000, SC_MS);

    return(0);
}
