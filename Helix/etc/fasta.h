/*
 *	Author: Anthony Hermans
 *	Date: 26/05/2015
 *	Information: All the functions about fasta
*/

#ifndef ____fasta__
#define ____fasta__

#include <stdio.h>
#include <string>
#include <vector>

/*
 *This function will replace certain symbols(that are not A,C,G,T) and convert them to A,C,G or T
*/
std::vector< std::string > fastaReplace(std::string str);

#endif /* defined(____fasta__) */
