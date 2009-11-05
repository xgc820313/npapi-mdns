/**
 * @file usocket.h
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#ifndef USOCKET_H_
#define USOCKET_H_

	#include <string>
	using namespace std;

	/**
	 * uSocket Exception Class
	 */
	class uSocketException {
	public:

		/**
		 * Create with an `errno`
		 */
		uSocketException( int _err ) {
			err = _err;
		}
		uSocketException( std::string s ) {
			m_desc=s;
		}
		~uSocketException();

		/**
		 * Returns the last `errno`
		 */
		int getError(void) {
			return err;
		}

	private:
		std::string m_desc;
		int err;
	};

	/**
	 * Unix domain socket
	 */
	class uSocket {

	protected:
		// sockets[0] is always used when serving
		// sockets[1] is sent to the peer
		int sockets[2];

		bool status;
		int  code;


	public:
		/**
		 * Creates a socket pair
		 */
		uSocket();

		/**
		 * Creates a single socket
		 */
		uSocket(int s);

		~uSocket();

		/**
		 * Returns the socket handle
		 * that should be used by the peer
		 */
		int getPeerSocket(void);

		int getLastError(void);

		/**
		 * Pops a "message" from the queue
		 */
		std::string popmsg(void);

		/**
		 * Pushes a "message" in the queue
		 */
		void pushmsg(std::string msg);
	};


#endif /* USOCKET_H_ */
