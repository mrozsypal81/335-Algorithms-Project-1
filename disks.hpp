///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_DARK, DISK_LIGHT };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_LIGHT;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    int current; //1 for light 0 for dark
    int temp;
    for ( auto color : _colors){
        if(color == DISK_DARK){
          current = 0;
        }
        if(color == DISK_LIGHT){
          current = 1;
        }
        if(current == temp){
          return false;
        }else{
          temp = current;
        }
      }
      return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the right (high indices) and all dark disks on the left (low
  // indices).
  bool is_sorted() const {
    int dark = 0;
    for (int x = 0; x < (total_count())/2;x++){
      if(_colors.at(x) == DISK_DARK){
        dark++;
      }
    }
    if(dark != (total_count())/2){
      return false;
    }else
    return true;

  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
	int swaps = 0;
        disk_state sorted(before.light_count());
	for (int n = 0; n < sorted.total_count()-1; n++) {
		for (int m = n; m < sorted.total_count()-1; m++) {
			if(sorted.get(m) != sorted.get(m+1)){
			   sorted.swap(m);
			 	swaps++;
			}
		}
	}
  return sorted_disks(sorted, swaps);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
	int swaps = 0;
	bool direction = true; //true is go right false is go left
	disk_state sorted(before.light_count());
		for (int n = 0; n < sorted.total_count()-1;){
		if(direction == true){
			for(int m = n; m < sorted.total_count()-1; m++){
				if(sorted.get(m) != sorted.get(m+1)){
				      sorted.swap(m);
					swaps++;
				}
			}
		direction = false;
		n++;
		}
		if(direction == false){
			for(int m = sorted.total_count()-n-1; m > n; m--){
				if(sorted.get(m) != sorted.get(m-1)){
				  	sorted.swap(m-1);
            swaps++;
				}
			}
		direction = true;
		n++;
		}

	}
  return sorted_disks(sorted, swaps);
}
