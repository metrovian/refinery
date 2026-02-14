#include "HUFF.h"
#include "HeaderHUFF.h"

bool HUFF::node::operator>(const node& _node)
{
	return this->freq > _node.freq;
}

bool HUFF::node::operator<(const node& _node)
{
	return this->freq < _node.freq;
}

bool HUFF::less(node* _node, node* _mode)
{
	return *_node < *_mode;
}

bool HUFF::greater(node* _node, node* _mode)
{
	return *_node > *_mode;
}

bool HUFF::free(node* _node)
{
	if (_node == nullptr) return false;

	free(_node->left);
	free(_node->right);

	delete _node;

	return true;
}

bool HUFF::match(node* _node, std::vector<bool> _code)
{
	if (_node == nullptr) return false;

	if (_node->left == nullptr && _node->right == nullptr)
	{
		code[_node->pval] = _code;
	}

	std::vector<bool> lcd = _code;
	std::vector<bool> rcd = _code;

	lcd.push_back(0);
	rcd.push_back(1);

	match(_node->left, lcd);
	match(_node->right, rcd);

	return true;
}

bool HUFF::decode(const std::string& _fname)
{
	HeaderHUFF header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderHUFF));

	if (header.type != 0x5548)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.depth != 0x0018)
	{
		std::cerr << "Header Error : " << header.depth << std::endl;
		return false;
	}

	width = header.width;
	height = header.height;

	comp.resize(header.dsi);

	std::vector<bool> cbts;
	std::priority_queue<node*, std::vector<node*>, decltype(&HUFF::greater)> heap(&HUFF::greater);

	uint8_t key = 0;
	uint64_t val = 0;

	ifs.read(reinterpret_cast<char*>(comp.data()), header.dsi);

	for (uint64_t i = 0; i < comp.size(); ++i)
	{
		for (uint64_t j = 0; j < 8; ++j)
		{
			cbts.push_back(comp[i] & (0x80 >> j));
		}
	}

	for (uint64_t i = 0; i < header.psi; ++i)
	{
		cbts.pop_back();
	}

	while
		(
			ifs.read(reinterpret_cast<char*>(&key), sizeof(uint8_t)) &&
			ifs.read(reinterpret_cast<char*>(&val), sizeof(uint64_t))
			)
	{
		heap.push(new node(key, val));
	}

	ifs.close();

	while (heap.size() > 1)
	{
		node* left = heap.top();
		heap.pop();

		node* right = heap.top();
		heap.pop();

		node* parent = new node(0, left->freq + right->freq);

		parent->left = left;
		parent->right = right;

		heap.push(parent);
	}

	node* root = heap.top();
	node* cur = root;

	heap.pop();

	raw.clear();

	for (uint64_t i = 0; i < cbts.size(); ++i)
	{
		if (cur != nullptr)
		{
			if (cbts[i]) cur = cur->right;
			else cur = cur->left;

			if (cur != nullptr)
			{
				if (cur->left == nullptr && cur->right == nullptr)
				{
					raw.push_back(cur->pval);
					cur = root;
				}
			}
		}
	}

	free(root);

	return true;
}

bool HUFF::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	std::unordered_map<uint8_t, uint64_t> map;
	std::priority_queue<node*, std::vector<node*>, decltype(&HUFF::greater)> heap(&HUFF::greater);

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		++map[raw[i]];
	}

	for (const auto& pair : map)
	{
		heap.push(new node(pair.first, pair.second));
	}

	while (heap.size() > 1)
	{
		node* left = heap.top();
		heap.pop();

		node* right = heap.top();
		heap.pop();

		node* parent = new node(0, left->freq + right->freq);

		parent->left = left;
		parent->right = right;

		heap.push(parent);
	}

	node* root = heap.top();
	heap.pop();

	code.clear();

	match(root, std::vector<bool>());
	free(root);

	std::vector<bool> cbts;

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		cbts.insert(cbts.end(), code[raw[i]].begin(), code[raw[i]].end());
	}

	uint8_t byte = 0x00;
	uint8_t count = 0x00;

	comp.clear();

	for (uint64_t i = 0; i < cbts.size(); ++i)
	{
		byte = cbts[i] + (byte << 1);
		++count;

		if (count == 0x08)
		{
			comp.push_back(byte);

			byte = 0x00;
			count = 0x00;
		}
	}

	if (count > 0)
	{
		byte <<= (0x08 - count);
		comp.push_back(byte);
	}

	HeaderHUFF header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	header.width = width;
	header.height = height;

	header.fsi = comp.size() + map.size() + header.ofs;
	header.dsi = comp.size();
	header.psi = 0x08 - count;

	ofs.write(reinterpret_cast<const char*>(&header), header.ofs);
	ofs.write(reinterpret_cast<const char*>(comp.data()), header.dsi);

	for (const auto& pair : map)
	{
		ofs.write(reinterpret_cast<const char*>(&pair.first), sizeof(uint8_t));
		ofs.write(reinterpret_cast<const char*>(&pair.second), sizeof(uint64_t));
	}

	ofs.close();

	return true;
}