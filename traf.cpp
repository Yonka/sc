#include "systemc.h"
#include "cstdlib"
#include "ctime"

class writeI: virtual public sc_interface
{
public:
	virtual void write(sc_uint<8> data) = 0;
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
		SC_METHOD(send_byte);
		sensitive << send;

	}
	void gen_event()
	{
		for (int r = 0; r < runs; r++)
		{
			for (int i = 0; i < 19; i++)
			{
				x = (sc_uint<8>)rand() % 255;
				send.notify();
				wait(5, SC_NS);	
			}
			x = 255;
			send.notify();
			wait(5, SC_NS);	
		}
	}
	void send_byte()
	{
		out_port->write(x);
//		cout << "send " << x << '\n';
	}
};

class receiver : public sc_module, public writeI 
{
public:
	sc_fifo<sc_uint<8> > tmp_buf;
	sc_uint<8> tmp_byte;
	sc_event eop;
	SC_HAS_PROCESS(receiver);
	receiver(sc_module_name mn) : sc_module(mn), tmp_buf(40)
	{
		SC_METHOD(printer);
		sensitive << eop;
	}

	virtual void write(sc_uint<8> data)
	{
		if (data == 255)
			eop.notify(0, SC_MS);
		else 
		tmp_buf.write(data);
	}

	void printer()
	{
		while (tmp_buf.nb_read(tmp_byte))
		{
			cout << tmp_byte << " ";
		}
		cout << '\n';
	}
};
int sc_main(int argc, char* argv[]) {
	int k = 1;
	if (argc == 2)
		k = atoi(argv[1]);
	trafgen t("t", k);
	receiver r("r");

	t.out_port(r);

	sc_start(2000, SC_MS);

	return(0);
}
