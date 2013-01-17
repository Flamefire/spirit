/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_OPTIONAL_MARCH_23_2007_1117PM)
#define SPIRIT_OPTIONAL_MARCH_23_2007_1117PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>
#include <boost/spirit/home/support/traits/attribute_of.hpp>
#include <boost/spirit/home/support/traits/move_to.hpp>
#include <boost/spirit/home/support/traits/optional_traits.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename Subject>
    struct optional : unary_parser<Subject, optional<Subject>>
    {
        typedef unary_parser<Subject, optional<Subject>> base_type;
        typedef typename traits::attribute_of<Subject>::type subject_attribute;
        static bool const handles_container = true;

        typedef typename
            traits::build_optional<subject_attribute>::type
        attribute_type;

        optional(Subject const& subject)
          : base_type(subject) {}

        // Attribute is a container
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr, mpl::true_) const
        {
            detail::parse_into_container(
                this->subject, first, last, context, attr);
            return true;
        }

        // Attribute is not a container
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr, mpl::false_) const
        {
            typedef typename
                spirit::traits::optional_value<Attribute>::type
            value_type;

            // create a local value
            value_type val = value_type();

            if (this->subject.parse(first, last, context, val))
            {
                // assign the parsed value into our attribute
                spirit::traits::move_to(val, attr);
            }
            return true;
        }

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr) const
        {
            typedef typename
                spirit::traits::optional_value<Attribute>::type
            value_type;

            return parse(first, last, context, attr
              , traits::is_container<value_type>());
        }
    };

    template <typename Subject>
    inline optional<typename extension::as_parser<Subject>::value_type>
    operator-(Subject const& subject)
    {
        typedef
            optional<typename extension::as_parser<Subject>::value_type>
        result_type;

        return result_type(as_parser(subject));
    }
}}}

#endif
