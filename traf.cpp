#include "systemc.h"
#include "cstdlib"
#include "ctime"

SC_MODULE (trafgen) 
{
	sc_port<sc_fifo_out_if<sc_uint<8> > > out_port;
	sc_uint<8> x;
	int runs;
	SC_HAS_PROCESS(trafgen);
	trafgen(sc_module_name mn, int param = 1) : sc_module(mn), runs(param)
	{
		SC_THREAD(send_byte);
	}
	void send_byte()
	{
		srand(time(0));
		for (int r = 0; r < runs; r++)
		{
			for (int i = 0; i < 19; i++)
			{
				x = (sc_uint<8>)rand() % 255;
				write_to_fifo();
			}
			x = 255;
			write_to_fifo();
		}
	}
	void write_to_fifo()
	{
		out_port->write(x);
//		cout << "send " << x << '\n';
		wait(5, SC_NS);	
	}
};

SC_MODULE (receiver) 
{
	sc_port<sc_fifo_in_if<sc_uint<8> > > in_port;
	sc_fifo<sc_uint<8> > tmp_buf;
	sc_uint<8> tmp_byte;
	SC_CTOR (receiver) : tmp_buf(40)
	{
		SC_THREAD(receive_byte);
//		sc_fifo<sc_uint<8> > tmp_buf(40);
	}
	void receive_byte()
	{
		while(true)
		{
			tmp_byte = in_port->read();
			if (tmp_byte == 255)
			{
				cout << "packet: ";
				printer();
			}
			else 
			{
				tmp_buf.write(tmp_byte);
//				cout << "rec " << tmp_byte << '\n';
			}
		}
	}
	void printer()
	{
		while (tmp_buf.nb_read(tmp_byte))
			cout << tmp_byte << " ";
		cout << '\n';
	}
};
int sc_main(int argc, char* argv[]) {
	int k = 1;
	if (argc == 2)
		k = atoi(argv[1]);
	trafgen t("t", k);
	receiver r("r");
	sc_fifo<sc_uint<8> > buf;
	t.out_port(buf);
	r.in_port(buf);

	sc_start(2000, SC_MS);

	return(0);
}
