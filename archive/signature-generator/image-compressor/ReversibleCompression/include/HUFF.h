#pragma once
#include "BMP.h"

class HUFF : public BMP
{
protected: /* struct */
	struct node
	{
	public: /* data */
		uint8_t pval = 0;
		uint64_t freq = 0;

	public: /* pointer */
		node* left = nullptr;
		node* right = nullptr;

	public: /* operator */
		bool operator>(const node& _node);
		bool operator<(const node& _node);

	public: /* constructor */
		node(uint8_t _pval, uint64_t _freq) : pval(_pval), freq(_freq) {};
	};

protected: /* data */
	std::unordered_map<uint8_t, std::vector<bool>> code;

protected: /* condition */
	static bool less(node* _node, node* _mode);
	static bool greater(node* _node, node* _mode);

protected: /* recursive */
	bool free(node* _node);
	bool match(node* _node, std::vector<bool> _code);

public: /* compression */
	virtual bool decode(const std::string& _fname);
	virtual bool encode(const std::string& _fname);
};