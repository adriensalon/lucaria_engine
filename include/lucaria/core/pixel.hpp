#pragma once

namespace lucaria {
namespace detail {

	enum struct pixel_format {
		red5_green6_blue5,
		red4_green4_blue4_alpha4,
		red5_green5_blue5_alpha1,
		red8_green8_blue8_alpha8,

		red8_green8_blue8_etc2,
		red8_green8_blue8_alpha8_etc2eac,
		red8_green8_blue8_dxt1,
		red8_green8_blue8_alpha8_dxt5,

		depth_16
	};

}
}
