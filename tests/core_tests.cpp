//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 17 May 2008 1:24:01 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define CATCH_CONFIG_MAIN 
#include "test/catch.hpp"
#include "core/memory/new.h"
#include "core/debug/assert.h"
#include "core/memory/dl_allocator.h"
#include "core/memory/allocator_2d.h"
#include "core/perfect_hash.h"
#include "core/file_utils.h"
#include "core/debug/assert.h"
#include "core/memory/new.h"
#include "core/containers/list.h"
#include "core/program_config.h"
#include "core/globals.h"
#include "core/string.h"
#include "core/core.h"
#include "test/global_test_fixture.h"
#include <vector>

using namespace tycho::test;
namespace tc = tycho::core; 

//////////////////////////////////////////////////////////////////////////////
// TEST CASES
//////////////////////////////////////////////////////////////////////////////

TEST_CASE("Basic list functionality", "")
{
	typedef tycho::core::list<int> list;
	list l;
	REQUIRE(l.size() == 0);
	for(int i = 0; i < 5; ++i)
		l.push_back(i);
	REQUIRE(l.size() == 5);	
	int count = 0;
	list::iterator it = l.begin();	
	while(it != l.end())	
	{
		++count;
		++it;
	}
	REQUIRE(count == 5);
	
	it = l.begin();
	count = 0;
	while(it != l.end())
	{
		REQUIRE(count == *it);
		++count;
		++it;
	}
	
	l.erase(l.begin());	
	REQUIRE(l.size() == 4);
	count = 0;
	it = l.begin();
	while(it != l.end())	
		++count, ++it;
	REQUIRE(count == 4);
}

typedef tc::program_config tcp;
	
		
TEST_CASE("test_program_options", "")
{
	const char* cmd_line = "program_name --test.foo=12 -c=0 --test.foso=1 -d=hello --test.f32=256.128";
	tc::program_config pc;
	pc.parse_command_line(cmd_line);
	int foo = 0;
	bool c = 0;
	bool foso = false;
	const char *d = 0;	
	float f32 = 0;
	
	// must be static as the program config class keeps a pointer to it for displaying
	// help info

	static tc::program_config::option_pair opts[] = 
		{
			tcp::opt("foo", 0, "test", "1", &foo),
			tcp::opt("fooc", 'c', "test", "1", &c),
			tcp::opt("foso", 0, "test", "0", &foso),
			tcp::opt("food", 'd', "test", "world", &d),
			tcp::opt("f32", 0, "test", "0.0", &f32),
			tcp::null_opt()
		};

	pc.process_option_group("test", "arse", opts);
	REQUIRE(foo == 12);
 	REQUIRE(c == false);
 	REQUIRE(foso == true);
 	REQUIRE(d);
 	REQUIRE(tc::strcmp(d, "hello"));
 	REQUIRE(f32 == 256.128f);
}


//TODO: Broken test
TEST_CASE("test_allocator_2d", "")
{
	using namespace tycho::core;
	allocator_2d alloc(512, 512);
	allocator_2d::tag* a0 = alloc.allocate(128, 256);
	REQUIRE(a0);
	allocator_2d::tag* a1 = alloc.allocate(128, 256);	
	REQUIRE(a1);
	allocator_2d::tag* a2 = alloc.allocate(128, 256);	
	REQUIRE(a2);
	allocator_2d::tag* a3 = alloc.allocate(300, 256); // force onto new vspan
	REQUIRE(a3);
	allocator_2d::tag* a4 = alloc.allocate(300, 256); // fail
	REQUIRE(!a4);
	a4 = alloc.allocate(200, 300); // fail
	REQUIRE(!a4);
	alloc.free(a2);
	alloc.free(a1);
	alloc.free(a0);
	alloc.free(a3);	
	// all space should be free here
	a0 = alloc.allocate(512, 512);
	REQUIRE(a0);
	alloc.free(a0);
	
	// thrash test to check free coalescing. allocate n variable sized strings then free and check we can allocate a full size texture
	int widths[] = { 16, 32, 8, 12, 24, 37 };
	int num_widths = sizeof(widths) / sizeof(widths[0]);
	int heights[] = { 16, 32, 8, 12, 24, 37 };
	int num_heights = sizeof(heights) / sizeof(heights[0]);	
	int num_allocs = 600;
	std::vector<allocator_2d::tag*> tags;
	::srand(0);
	for(int i = 0; i < num_allocs; ++i)
	{
		float r0 = float(::rand()) / RAND_MAX;
		float r1 = float(::rand()) / RAND_MAX;
		int w = widths[(int)(r0 * (num_widths-1))];
		int h = heights[(int)(r1 * (num_heights-1))];
		tags.push_back(alloc.allocate(w, h));
	}
	std::random_shuffle(tags.begin(), tags.end());
	for(size_t i = 0; i < tags.size(); ++i)
	{
		alloc.free(tags[i]);
	}
	tags.clear();
	// all space should be free here
	a0 = alloc.allocate(512, 512);
	REQUIRE(a0);
	alloc.free(a0);
	
}

TEST_CASE("test_dl_allocator", "")
{
	//=====================================================
	// Doug Lea Allocator Tests
	//=====================================================
	using namespace tycho::core;

	dl_allocation_summary pre_summary, post_summary = {0};

	// allocator with 1KB of space
	dl_allocator dlalloc(1000);

	// this should succeed
	static const int szsmall = 50;
	pre_summary = dlalloc.get_alloction_summary();
	void* small = dlalloc.malloc(szsmall);
	REQUIRE(small);

	// test size allocated is more than 50 (padding will vary depending footer and pointer size)
	post_summary = dlalloc.get_alloction_summary();
	REQUIRE((post_summary.allocated - pre_summary.allocated) >= szsmall);

	size_t sz = dlalloc.get_allocation_size(small);

	// free
	dlalloc.free(small);
	post_summary = dlalloc.get_alloction_summary();
	REQUIRE(post_summary.allocated == pre_summary.allocated);

	// this should fail if we are only using MSPACES
	// otherwise we have requested additional mem from the OS
	void* big = dlalloc.malloc(1001);
	if (dlalloc.can_grow())
	{
		REQUIRE(big);
		sz = dlalloc.get_allocation_size(big);
	}
	else
	{
		REQUIRE(!big);
	}

	// this should succeed, ie our address is on a 32 byte boundary
	void* aligned32 = dlalloc.malloc_aligned(33, 32);
	REQUIRE(((std::ptrdiff_t)(aligned32) % 32) == 0);
	sz = dlalloc.get_allocation_size(aligned32);
	dlalloc.free(aligned32);

	// test overrun - this will assert if asserts enabled
	static int szoverrun = 20;
	void* overrun = dlalloc.malloc(szoverrun);
	size_t realszoverrun = dlalloc.get_allocation_size(overrun);
	uint8* ptr = reinterpret_cast<uint8*>(overrun);
	for (int i = 0; i <= realszoverrun; ++i)
	{
		*ptr = static_cast<uint8>(i);
		++ptr;
	}
	dlalloc.free(overrun);
}

TEST_CASE("test_debug_allocator", "")
{
	//=====================================================
	// Debug Allocator Tests
	//=====================================================
	using namespace tycho::core;

	// check alignment working with debug allocator
	void* debugaligned = allocator::malloc_aligned(33, 32);
	REQUIRE(((std::ptrdiff_t)(debugaligned) % 32) == 0);
	allocator::get_allocation_size(debugaligned);
	allocator::free(debugaligned);
}

