#pragma once

namespace Angine {

	class ImageLoader {
	public:
		static GLTexture LoadPNG(std::string FilePath);
	};

}