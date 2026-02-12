#include "JPEG.h"
#include "HeaderJPEG.h"

#include "jpeglib.h"

bool JPEG::decode(const std::string& _fname)
{
	HeaderJPEG header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.seekg(0, std::ios::end);

	comp.clear();
	comp.resize(ifs.tellg(), 0);

	ifs.seekg(0, std::ios::beg);
	ifs.read(reinterpret_cast<char*>(comp.data()), comp.size());

	if (comp[1] + (comp[0] << 8) != header.type)
	{
		std::cerr << "Header Error : " << header.colc << std::endl;
		return false;
	}

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, comp.data(), comp.size());

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	header.width = cinfo.output_width;
	header.height = cinfo.output_height;

	width = header.width;
	height = header.height;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height - 1);

	std::vector<uint8_t> row(width * 3);

	if (cinfo.output_components != header.colc)
	{
		std::cerr << "Header Error : " << header.colc << std::endl;
		return false;
	}

	raw.clear();
	raw.resize(pwidth * height, 0);

	JSAMPROW ptrs = row.data();

	for (uint64_t i = 0; i < height; ++i)
	{
		jpeg_read_scanlines(&cinfo, &ptrs, 1);

		for (uint64_t j = 0; j < width; ++j)
		{
			raw[(pheight - i) * pwidth + j * 3 + 0] = row[j * 3 + 2];
			raw[(pheight - i) * pwidth + j * 3 + 1] = row[j * 3 + 1];
			raw[(pheight - i) * pwidth + j * 3 + 2] = row[j * 3 + 0];
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return true;
}

bool JPEG::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	HeaderJPEG header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	unsigned char* ptro = nullptr;
	unsigned long csi = 0;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height - 1);

	std::vector<uint8_t> row(width * 3);

	jpeg_create_compress(&cinfo);
	jpeg_mem_dest(&cinfo, &ptro, &csi);

	cinfo.err = jpeg_std_error(&jerr);

	cinfo.image_width = width;
	cinfo.image_height = height;

	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 75, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	JSAMPROW ptrs = row.data();

	for (uint64_t i = 0; i < height; ++i)
	{
		for (uint64_t j = 0; j < width; ++j)
		{
			row[j * 3 + 0] = raw[(pheight - i) * pwidth + j * 3 + 2];
			row[j * 3 + 1] = raw[(pheight - i) * pwidth + j * 3 + 1];
			row[j * 3 + 2] = raw[(pheight - i) * pwidth + j * 3 + 0];
		}

		jpeg_write_scanlines(&cinfo, &ptrs, 1);
	}

	jpeg_finish_compress(&cinfo);

	comp.clear();
	comp.resize(csi);

	std::memcpy(comp.data(), ptro, csi);

	jpeg_destroy_compress(&cinfo);

	ofs.write(reinterpret_cast<const char*>(comp.data()), csi);
	ofs.close();

	return true;
}
