/*****************************************************************************/
/**
 * @file makeString.h
 *
 * @class makeString
 *
 * @brief Classe outil permettant de formatter rapidement une chaine
 *
 *
 * @author
 *    SDE   08/07/2010
 *
 *
 */
/*****************************************************************************/

#pragma once
#ifndef MAKESTRING_H_H
#define MAKESTRING_H_H

#include <string>
#include <sstream>

namespace tools
{

	/**
	 * @addtogroup logging
	 */
	//@{
	/**
	 * Tool class allowing to quickly format a string 
	 *
	 * usage example: 
	 * @code
	 *  void f( std::string const& );
	 *  int main()
	 *  {
	 *    std::string name = "David";
	 *    f( makeString() << "Hello " << name << "!" );
	 *  }
	 * @endcode
	 * origin :
	 * http://stackoverflow.com/questions/469696/what-is-your-most-useful-c-c-utility/470999#470999
	 */
	class makeString
	{
	public:
		makeString()
		{
		}

		template<typename T>
		makeString& operator<<( T const& datum )
		{
			m_buffer << datum;
			return *this;
		}

		operator std::string() const
		{
			return m_buffer.str();
		}

		/*operator std::ostream&()
		{
			return m_buffer;
		}*/
	private:
		std::ostringstream m_buffer;

		// noncopyable, copying a makeString object is probably an error
		// consider 'casting' it explicitly to std::string if needed
		// example:
		// someTemplatedStuffWhichTriesToCopyMyMakeString( std::string( tools::makeString() << "hello " << "world! " << 42 ) );
		makeString( const makeString& );
		makeString& operator=( const makeString& );

		friend std::ostream&    operator<<( std::ostream& ostr, const makeString& o ) { return ostr << o.m_buffer.str(); }
	};

//@}

}// namespace tools

#endif // MAKESTRING_H_H
