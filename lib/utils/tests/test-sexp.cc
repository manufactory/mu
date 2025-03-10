/*
** Copyright (C) 2020 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public License
**  as published by the Free Software Foundation; either version 2.1
**  of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
**  02110-1301, USA.
*/

#include <vector>
#include <glib.h>

#include <iostream>
#include <sstream>

#include "mu-command-parser.hh"
#include "mu-utils.hh"

using namespace Mu;

static bool
check_parse(const std::string& expr, const std::string& expected)
{
	try {
		const auto parsed{to_string(Sexp::make_parse(expr))};
		assert_equal(parsed, expected);
		return true;

	} catch (const Error& err) {
		g_warning("caught exception parsing '%s': %s", expr.c_str(), err.what());
		return false;
	}
}

static void
test_parser()
{
	check_parse(":foo-123", ":foo-123");
	check_parse("foo", "foo");
	check_parse(R"(12345)", "12345");
	check_parse(R"(-12345)", "-12345");
	check_parse(R"((123 bar "cuux"))", "(123 bar \"cuux\")");

	check_parse(R"("foo\"bar\"cuux")", "\"foo\\\"bar\\\"cuux\"");

	check_parse(R"("foo
bar")",
		    "\"foo\nbar\"");
}

static void
test_list()
{
	const auto nstr{Sexp::make_string("foo")};
	g_assert_true(nstr.value() == "foo");
	g_assert_true(nstr.type() == Sexp::Type::String);
	assert_equal(nstr.to_sexp_string(), "\"foo\"");

	const auto nnum{Sexp::make_number(123)};
	g_assert_true(nnum.value() == "123");
	g_assert_true(nnum.type() == Sexp::Type::Number);
	assert_equal(nnum.to_sexp_string(), "123");

	const auto nsym{Sexp::make_symbol("blub")};
	g_assert_true(nsym.value() == "blub");
	g_assert_true(nsym.type() == Sexp::Type::Symbol);
	assert_equal(nsym.to_sexp_string(), "blub");

	Sexp::List list;
	list.add(Sexp::make_string("foo"))
	    .add(Sexp::make_number(123))
	    .add(Sexp::make_symbol("blub"));

	const auto nlst = Sexp::make_list(std::move(list));
	g_assert_true(nlst.list().size() == 3);
	g_assert_true(nlst.type() == Sexp::Type::List);
	g_assert_true(nlst.list().at(1).value() == "123");

	assert_equal(nlst.to_sexp_string(), "(\"foo\" 123 blub)");
}

static void
test_prop_list()
{
	Sexp::List l1;
	l1.add_prop(":foo", Sexp::make_string("bar"));
	Sexp s2{Sexp::make_list(std::move(l1))};
	assert_equal(s2.to_sexp_string(), "(:foo \"bar\")");
	g_assert_true(s2.is_prop_list());

	Sexp::List        l2;
	const std::string x{"bar"};
	l2.add_prop(":foo", Sexp::make_string(x));
	l2.add_prop(":bar", Sexp::make_number(77));
	Sexp::List l3;
	l3.add_prop(":cuux", Sexp::make_list(std::move(l2)));
	Sexp s3{Sexp::make_list(std::move(l3))};
	assert_equal(s3.to_sexp_string(), "(:cuux (:foo \"bar\" :bar 77))");
}

static void
test_props()
{
	auto sexp2 = Sexp::make_list(Sexp::make_string("foo"),
				     Sexp::make_number(123),
				     Sexp::make_symbol("blub"));

	auto sexp = Sexp::make_prop_list(":foo",
					 Sexp::make_string("bär"),
					 ":cuux",
					 Sexp::make_number(123),
					 ":flub",
					 Sexp::make_symbol("fnord"),
					 ":boo",
					 std::move(sexp2));

	assert_equal(sexp.to_sexp_string(),
		     "(:foo \"b\303\244r\" :cuux 123 :flub fnord :boo (\"foo\" 123 blub))");
}

static void
test_prop_list_remove()
{
	{
		Sexp::List lst;
		lst.add_prop(":foo", Sexp::make_string("123"))
			.add_prop(":bar", Sexp::make_number(123));

		assert_equal(Sexp::make_list(std::move(lst)).to_sexp_string(),
			     R"((:foo "123" :bar 123))");
	}

	{
		Sexp::List lst;
		lst.add_prop(":foo", Sexp::make_string("123"))
			.add_prop(":bar", Sexp::make_number(123));

		assert_equal(Sexp::make_list(Sexp::List{lst}).to_sexp_string(),
			     R"((:foo "123" :bar 123))");

		lst.remove_prop(":bar");

		assert_equal(Sexp::make_list(Sexp::List{lst}).to_sexp_string(),
			     R"((:foo "123"))");

		lst.clear();
		g_assert_cmpuint(lst.size(), ==, 0);
	}

	{
		Sexp::List lst;
		lst.add(Sexp::make_number(123));
		Sexp s2{Sexp::make_list(std::move(lst))};
		g_assert_false(s2.is_prop_list());
	}
}

int
main(int argc, char* argv[])
try {
	g_test_init(&argc, &argv, NULL);

	if (argc == 2) {
		std::cout << Sexp::make_parse(argv[1]) << '\n';
		return 0;
	}

	g_test_add_func("/utils/sexp/parser", test_parser);
	g_test_add_func("/utils/sexp/list", test_list);
	g_test_add_func("/utils/sexp/proplist", test_prop_list);
	g_test_add_func("/utils/sexp/proplist-remove", test_prop_list_remove);
	g_test_add_func("/utils/sexp/props", test_props);

	return g_test_run();

} catch (const std::runtime_error& re) {
	std::cerr << re.what() << "\n";
	return 1;
}
