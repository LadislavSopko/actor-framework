/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011, 2012                                                   *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License                  *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef GET_VIEW_HPP
#define GET_VIEW_HPP

#include <vector>
#include <cstddef>

#include "cppa/tuple.hpp"
#include "cppa/pattern.hpp"
#include "cppa/anything.hpp"
#include "cppa/any_tuple.hpp"
#include "cppa/tuple_view.hpp"

#include "cppa/util/tbind.hpp"
#include "cppa/util/type_list.hpp"

#include "cppa/detail/matches.hpp"

namespace cppa {

template<typename... MatchRules>
typename tuple_view_type_from_type_list<typename util::tl_filter_not<util::type_list<MatchRules...>, util::tbind<std::is_same, anything>::type>::type>::type
get_view(any_tuple const& ut)
{
    pattern<MatchRules...> p;
    typename pattern<MatchRules...>::mapping_vector mapping;
    if (detail::matches(detail::pm_decorated(ut.begin(), &mapping), p.begin()))
    {
        return { ut.vals(), mapping };
    }
    // todo: throw nicer exception
    throw std::runtime_error("doesn't match");
}

} // namespace cppa

#endif // GET_VIEW_HPP
