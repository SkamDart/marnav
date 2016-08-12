#ifndef __NMEA__RTE__HPP__
#define __NMEA__RTE__HPP__

#include <marnav/nmea/sentence.hpp>
#include <marnav/nmea/waypoint.hpp>
#include <marnav/utils/optional.hpp>

namespace marnav
{
namespace nmea
{
MARNAV_NMEA_DECLARE_SENTENCE_PARSE_FUNC(rte)

/// @brief RTE - Routes
///
/// @code
///        1   2   3 4    5           x
///        |   |   | |    |           |
/// $--RTE,x.x,x.x,a,c--c,c--c, ..... c--c*hh<CR><LF>
/// @endcode
///
/// Field Number:
/// 1. Total number of messages being transmitted
/// 2. Message Number
/// 3. Message mode c = complete route, all waypoints w = working route, the waypoint
///    you just left, the waypoint you’re heading to, then all the rest
/// 4. Waypoint ID
///
/// More waypoints follow. Last field is a checksum as usual.
///
/// The Garmin 65 and possibly other units report a <tt>$GPR00</tt> in the same format.
///
class rte : public sentence
{
	MARNAV_NMEA_SENTENCE_FRIENDS(rte)

public:
	constexpr static const sentence_id ID = sentence_id::RTE;
	constexpr static const char * TAG = "RTE";

	constexpr static int max_waypoints = 10;

	virtual ~rte() {}

	rte();
	rte(const rte &) = default;
	rte & operator=(const rte &) = default;

protected:
	rte(const std::string & talker, fields::const_iterator first, fields::const_iterator last);
	virtual std::vector<std::string> get_data() const override;

private:
	uint32_t n_messages = 1;
	uint32_t message_number = 1;
	route message_mode = route::complete; // C:complete route, W:working route
	utils::optional<waypoint>
		waypoint_id[max_waypoints]; // names or numbers of the active route

public:
	NMEA_GETTER(n_messages)
	NMEA_GETTER(message_number)
	NMEA_GETTER(message_mode)
	utils::optional<waypoint> get_waypoint_id(int index) const;

	void set_n_messages(uint32_t t) noexcept { n_messages = t; }
	void set_message_number(uint32_t t) noexcept { message_number = t; }
	void set_message_mode(route t) noexcept { message_mode = t; }
	void set_waypoint_id(int index, const waypoint & id);
};
}
}

#endif
