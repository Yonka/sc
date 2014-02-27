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

class receiver : public sc_module, public writeI 
{
public:
	sc_fifo<sc_uint<8> > tmp_buf;
	sc_uint<8> tmp_byte;

	SC_HAS_PROCESS(receiver);
	receiver(sc_module_name mn) : sc_module(mn), tmp_buf(40)
	{
		SC_THREAD(receive_byte);
	}

	void receive_byte()
	{
		while(true)
		{
			if (tmp_buf.num_available() < 20)
				wait(tmp_buf.data_written_event());
			else
				printer();
		}
	}

	virtual void write(sc_uint<8> data)
	{
		tmp_buf.write(data);
	}

	void printer()
	{
		tmp_buf.read(tmp_byte);
		while (tmp_byte != 255)
		{
			cout << tmp_byte << " ";
			if (!tmp_buf.nb_read(tmp_byte))
				break;
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
