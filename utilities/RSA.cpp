//
// Created by tomasz on 10.06.16.
//

#include "RSA.h"
namespace notifications
{
	unsigned RSA::e; // składowa klucza publicznego
	unsigned RSA::d; // składowa klucza prywatnego
	unsigned RSA::n;

	RSA::RSA()
	{

		e = 981967781U;
		d = 2245171601U;
		n = 4294894073U;


		/*
        #ifdef CLIENT
            partner_n = 4294894073U;
            partner_e = 981967781U;
        #endif
        */
	}

	unsigned RSA::encrypt(unsigned given, unsigned ee, unsigned nn)
	{
		return ModExp(given,ee,nn);
	}

	unsigned RSA::decrypt(unsigned given, unsigned dd, unsigned nn)
	{
		std::cout << e << " " << d << " " << n << std::endl;
		return ModExp(given,dd,nn);
	}

	int RSA::encrypt(std::string msg, char *& output, unsigned ee, unsigned nn)
	{
		return crypt(msg.c_str(), output, ee, nn, msg.size());
	}

	std::string RSA::decrypt(const char * input, int length, unsigned dd, unsigned nn)
	{
		char * output;
		crypt(input, output, dd, nn, length);
		std::string temp(output);
		delete output;
		return temp;
	}

	void RSA::getPublicKey(unsigned & ee, unsigned & nn)
	{
		ee = e;
		nn = n;
	}

	void RSA::setNewKey(unsigned ee, unsigned dd, unsigned nn)
	{
		e = ee;
		d = dd;
		n = nn;
	}

	void RSA::setNewKey()
	{
		while(1){
			unsigned p, q = 1;
			unsigned long long mul;
			unsigned phi;
			bool ready = false;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned> pq(pq_lower_bound,pq_upper_bound);
			std::uniform_int_distribution<unsigned> e_key(e_lower_bound,e_upper_bound);
			do{ // generate pseudorandom p and q
				p = pq(gen);
				p|=1;

				while(!is_prime(p)) p+=2;

				mul = p*q;
				// check modulo= p*q
				if(ready && (mul > n_upper_bound || mul < n_lower_bound ))
				{
					ready = false;
					q = 1;
					continue;
				}
				if(ready) break;
				ready = true;
				q = p;
			}while(1); // generate pseudorandom p and q

			n = p*q;

			phi = (p-1)*(q-1);
			// find prime number in field phi - e (e is odd)
			e = e_key(gen)%(phi);
			if(e%2 == 0) ++e;
			while(!gcd_1(e,phi)) e=(e+2)%(phi);
			// e was found
			// find inversion of e in field phi - d
			d = getInverse(e, phi);
			// some output and testing;

			// since this place p q phi are used as temporary variables
			// start testing the key
			unsigned pp, qq, hh;
			pp=201;
			qq=ModExp(pp,e,n);
			hh=ModExp(qq,d,n);

			if(pp!=hh)
			{
				//std::cout<<"Wrong!" << std::endl;
				continue;
			}

			// test rsa key
			for(unsigned i=1; i<=10; i++)
			{
				pp+=pp*i;
				qq=ModExp(pp,e,n);
				hh=ModExp(qq,d,n);

				if(pp!=hh) break;
			}
			if(pp!=hh) continue;
			else
			{
				std::cout<<"p = " << p << ", \tq = " << q << ", \tn = " << n << std::endl;
				std::cout<<"e = " << e << ", d = " << d <<", phi = " << phi << std::endl;
				break;
			}
		}

	}

/*
 * Miller Rabin pseudorandom test as described on page 8 at:
 * http://cacr.uwaterloo.ca/hac/about/chap4.pdf
 */
	bool RSA::is_prime(unsigned n)
	{
		int t = 2;
		unsigned s = 0;
		unsigned n_1 = n-1;
		unsigned r = n_1;
		unsigned y, a;
		// check https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Deterministic_variants
		// if n < 1,373,653, it is enough to test a = 2 and 3
		unsigned a_array[] = {2, 3};
		// 1. Write n-1 = 2^s*r such that r is odd
		while(!r&1) // do when r is even
		{
			++s;
			r>>=1;
		}
		// 2. For i from 1 to t do the following
		for(int i = 0; i<t; ++i)
		{
			// Choose a random integer a, 2<=a<=n-2
			// select a from a_array
			a = a_array[i];
			// Compute  y = a^r mod n
			y = ModExp(a,r,n);
			if(y!=1 && y!=n_1)
			{
				for(unsigned j = 1; j<s && y!=n_1; ++j)
				{
					//y = y^2 mod n;
					y = ModExp(y,2,n);
					if(y==1) return false;
				}
				if(y!=n_1) return false;
			}
		}
		// 3. Miller-Rabin probabilistic primality test passed
		return true;
	}

	int RSA::crypt(const char * given, char * &out, unsigned power, unsigned modul, int len)
	{
		if(len<=0)
			len = strlen(given);
		int padding=0;
		if(len%4!=0) padding = 4 -len%4;
		const int flen = len+padding;
		char * buf = new char[flen];
		for(int yu=0; yu < len; ++yu)
		{
			buf[yu] = given[yu];
		}
		for(int yu=len; yu<flen; ++yu)
		{
			buf[yu] = '\0';
		}

		int i = flen/4;
		unsigned * arr = new unsigned[i];
		char * hhh = new char[flen];
		for(int k=0; k<i; ++k)
		{
			arr[k] = *((unsigned*)(given+k*4));
			*((unsigned*)(hhh+k*4)) = ModExp(arr[k], power, modul);
		}
		out = hhh;
		return flen;
	}

	unsigned RSA::getInverse(unsigned b, unsigned m)
	{
		long long m0 = m, t, q;
		long long x0 = 0, x1 = 1;
		long long a = (long long)b;
		if (m == 1)
			return 0;

		while (a > 1)
		{
			// q is quotient
			q = a / m;

			t = m;

			// m is remainder now, process same as
			// Euclid's algo
			m = a % m, a = t;

			t = x0;

			x0 = x1 - q * x0;

			x1 = t;
		}

		// Make x1 positive
		if (x1 < 0)
			x1 += m0;

		return (unsigned int)x1;
	}

	bool RSA::gcd_1(unsigned a, unsigned b)
	{
		unsigned temp;

		while(b != 0)
		{
			temp = b;
			b = a % b;
			a = temp;
		};
		return (a == 1);
	}


/* This function calculates (a^b)%c */
	unsigned RSA::ModExp(unsigned a,unsigned b,unsigned c){
		unsigned long long x=1,y=a; // long long is taken to avoid overflow of intermediate results
		while(b > 0){
			if(b%2 == 1){
				x=(x*y)%c;
			}
			y = (y*y)%c; // squaring the base
			b /= 2;
		}
		return x%c;
	}
}