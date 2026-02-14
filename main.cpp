#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {
struct BenchResult {
	std::size_t n;
	double moore_us;
	double sort_us;
	double brute_us;
};

volatile int g_sink = 0;

int find_majority_moore(const std::vector<int> &data) {
	int candidate = 0;
	int count = 0;
	for (int value : data) {
		if (count == 0) {
			candidate = value;
			count = 1;
		} else if (value == candidate) {
			++count;
		} else {
			--count;
		}
	}

	std::size_t occurrences = 0;
	for (int value : data) {
		if (value == candidate) {
			++occurrences;
		}
	}

	return (occurrences > data.size() / 2) ? candidate : -1;
}

int find_majority_sort(std::vector<int> data) {
	std::sort(data.begin(), data.end());
	int candidate = data[data.size() / 2];
	std::size_t occurrences = 0;
	for (int value : data) {
		if (value == candidate) {
			++occurrences;
		}
	}
	return (occurrences > data.size() / 2) ? candidate : -1;
}

int find_majority_bruteforce(const std::vector<int> &data) {
	std::size_t n = data.size();
	std::size_t best_count = 0;
	int best_value = -1;
	for (std::size_t i = 0; i < n; ++i) {
		std::size_t count = 0;
		for (std::size_t j = 0; j < n; ++j) {
			if (data[j] == data[i]) {
				++count;
			}
		}
		if (count > best_count) {
			best_count = count;
			best_value = data[i];
		}
	}

	return (best_count > n / 2) ? best_value : -1;
}

std::vector<std::size_t> build_log_checkpoints(std::size_t max_n) {
	std::vector<std::size_t> checkpoints;
	std::size_t base = 1;
	while (base <= max_n) {
		for (std::size_t factor : {1u, 2u, 5u}) {
			std::size_t value = base * factor;
			if (value <= max_n) {
				checkpoints.push_back(value);
			}
		}
		if (base > max_n / 10) {
			break;
		}
		base *= 10;
	}
	return checkpoints;
}

template <typename Func>
double time_us(Func &&func, int repeats) {
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < repeats; ++i) {
		func();
	}
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(end - start);
	return elapsed.count() / static_cast<double>(repeats);
}

void write_html_report(const std::string &path, const std::vector<BenchResult> &results) {
	std::ofstream out(path);
	out << "<!doctype html>\n";
	out << "<html lang=\"en\">\n";
	out << "<head>\n";
	out << "  <meta charset=\"utf-8\">\n";
	out << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
	out << "  <title>Majority Benchmark</title>\n";
	out << "  <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n";
	out << "  <style>\n";
	out << "    :root { --bg: #f3efe8; --ink: #201d16; --accent: #e65c2f; --accent2: #2f6fe6; --accent3: #2f9e44; }\n";
	out << "    body { margin: 0; font-family: 'Georgia', serif; background: radial-gradient(circle at top, #fff6e7 0%, var(--bg) 55%, #eadfcf 100%); color: var(--ink); }\n";
	out << "    .wrap { max-width: 980px; margin: 0 auto; padding: 40px 24px 56px; }\n";
	out << "    h1 { font-size: 2.3rem; letter-spacing: 0.02em; margin-bottom: 12px; }\n";
	out << "    p { margin-top: 0; max-width: 720px; }\n";
	out << "    .card { background: rgba(255,255,255,0.7); border-radius: 18px; padding: 18px 18px 26px; box-shadow: 0 20px 40px rgba(0,0,0,0.08); }\n";
	out << "    canvas { width: 100%; height: 420px; }\n";
	out << "  </style>\n";
	out << "</head>\n";
	out << "<body>\n";
	out << "  <div class=\"wrap\">\n";
	out << "    <h1>Majority Element Benchmark</h1>\n";
	out << "    <p>Timing results for Moore voting, sort-based, and brute force majority detection with a growing array up to 1,000,000 elements (brute force capped at 50,000).</p>\n";
	out << "    <div class=\"card\">\n";
	out << "      <canvas id=\"benchChart\"></canvas>\n";
	out << "    </div>\n";
	out << "  </div>\n";
	out << "  <script>\n";
	out << "    const labels = [";
	for (std::size_t i = 0; i < results.size(); ++i) {
		if (i > 0) {
			out << ", ";
		}
		out << "'" << results[i].n << "'";
	}
	out << "];\n";
	out << "    const data = {\n";
	out << "      labels,\n";
	out << "      datasets: [\n";
	out << "        { label: 'Moore voting (us)', data: [";
	for (std::size_t i = 0; i < results.size(); ++i) {
		if (i > 0) {
			out << ", ";
		}
		out << std::fixed << std::setprecision(2) << results[i].moore_us;
	}
	out << "], borderColor: '#e65c2f', backgroundColor: 'rgba(230,92,47,0.15)', tension: 0.25 },\n";
	out << "        { label: 'Sort-based (us)', data: [";
	for (std::size_t i = 0; i < results.size(); ++i) {
		if (i > 0) {
			out << ", ";
		}
		out << std::fixed << std::setprecision(2) << results[i].sort_us;
	}
	out << "], borderColor: '#2f6fe6', backgroundColor: 'rgba(47,111,230,0.12)', tension: 0.25 },\n";
	out << "        { label: 'Brute force (us)', data: [";
	for (std::size_t i = 0; i < results.size(); ++i) {
		if (i > 0) {
			out << ", ";
		}
		if (results[i].brute_us < 0.0) {
			out << "null";
		} else {
			out << std::fixed << std::setprecision(2) << results[i].brute_us;
		}
	}
	out << "], borderColor: '#2f9e44', backgroundColor: 'rgba(47,158,68,0.12)', tension: 0.25 }\n";
	out << "      ]\n";
	out << "    };\n";
	out << "\n";
	out << "    new Chart(document.getElementById('benchChart'), {\n";
	out << "      type: 'line',\n";
	out << "      data,\n";
	out << "      options: {\n";
	out << "        responsive: true,\n";
	out << "        plugins: { legend: { position: 'bottom' } },\n";
	out << "        scales: { y: { type: 'logarithmic', title: { display: true, text: 'Time (microseconds, log)' } }, x: { title: { display: true, text: 'Array size' } } }\n";
	out << "      }\n";
	out << "    });\n";
	out << "  </script>\n";
	out << "</body>\n";
	out << "</html>\n";
}
} // namespace

int main() {
	std::mt19937 rng(42);
	std::uniform_int_distribution<int> dist(2, 1'000'000);
	std::vector<BenchResult> results;
	const std::size_t max_n = 1'000'000;
	const std::size_t brute_cap = 50'000;
	const int repeats = 10;
	const std::vector<std::size_t> checkpoints = build_log_checkpoints(max_n);
	std::size_t checkpoint_index = 0;

	std::cout << std::left << std::setw(12) << "Size"
			  << std::setw(16) << "Moore (us)"
			  << std::setw(16) << "Sort (us)"
			  << std::setw(16) << "Brute (us)" << "\n";
	std::cout << std::string(60, '-') << "\n";

	std::vector<int> data;
	data.reserve(max_n);
	std::size_t majority_count = 0;

	for (std::size_t n = 1; n <= max_n; ++n) {
		if (majority_count <= n / 2) {
			data.push_back(1);
			++majority_count;
		} else {
			data.push_back(dist(rng));
		}

		if (checkpoint_index < checkpoints.size() && n == checkpoints[checkpoint_index]) {
			std::shuffle(data.begin(), data.end(), rng);
			BenchResult row{n, 0.0, 0.0, 0.0};
			row.moore_us = time_us([&]() { g_sink = find_majority_moore(data); }, repeats);
			row.sort_us = time_us([&]() { g_sink = find_majority_sort(data); }, repeats);
			if (n <= brute_cap) {
				row.brute_us = time_us([&]() { g_sink = find_majority_bruteforce(data); }, repeats);
			} else {
				row.brute_us = -1.0;
			}
			results.push_back(row);

			std::cout << std::left << std::setw(12) << row.n
					  << std::setw(16) << std::fixed << std::setprecision(2) << row.moore_us
					  << std::setw(16) << std::fixed << std::setprecision(2) << row.sort_us;
			if (row.brute_us < 0.0) {
				std::cout << std::setw(16) << "N/A";
			} else {
				std::cout << std::setw(16) << std::fixed << std::setprecision(2) << row.brute_us;
			}
			std::cout << "\n";

			++checkpoint_index;
		}
	}

	write_html_report("benchmark.html", results);
	std::cout << "\nWrote benchmark.html\n";
	return 0;
}
