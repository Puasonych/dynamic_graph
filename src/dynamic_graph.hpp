#include <cstdint>
#include <vector>
#include <utility>
#include <cmath>
#include <random>
#include <tuple>
#include <map>
#include <algorithm>

std::random_device rd;

std::mt19937 mt(rd());

namespace prime {

    bool is_prime(int64_t value)
    {
        if (value == 2)
        {
            return true;
        }

        if (value % 2 == 0)
        {
            return false;
        }

        int64_t step = 3;
        while (step * step <= value)
        {
            if (value % step == 0)
            {
                return false;
            }

            step += 2;
        }

        return true;
    }

    int64_t prime_more_than(int64_t value)
    {
        while (!is_prime(value))
        {
            ++value;
        }

        return value;
    }
}

namespace l0sample {

    std::map<int64_t, int64_t> cache_prime;

    int64_t prime_more_than(int64_t value)
    {
        auto search = cache_prime.find(value);

        if (search == cache_prime.end())
        {
            cache_prime[value] = prime::prime_more_than(4 * value);
        }

        return cache_prime[value];
    }

    int64_t fast_pow(int64_t value, int64_t p_value, int64_t mod)
    {
        if (p_value == 0)
        {
            return 1;
        }

        if (p_value == 1)
        {
            return value;
        }

        int64_t mult = (p_value % 2 == 1 ? value : 1);

        return mult * fast_pow((value * value) % mod, p_value / 2, mod);
    }

    class hash_k
    {
    public:
        explicit hash_k(int64_t dom, int64_t k_value = 2)
            : m_dom(dom),
            m_prime_value(prime::prime_more_than(2 * dom))
        {
            std::uniform_int_distribution<int64_t> rand_int64_t(0, m_prime_value - 1);

            for (auto i = 0u; i < k_value; ++i)
            {
                m_coefficients.push_back(rand_int64_t(mt));
            }
        }

        hash_k()
            : m_dom(0), m_prime_value(0)
        {
        }

        hash_k(const hash_k & hash)
        {
            m_dom = hash.m_dom;
            m_prime_value = hash.m_prime_value;
            m_coefficients = hash.m_coefficients;
        }

        hash_k & operator=(const hash_k & other)
        {
            m_dom = other.m_dom;
            m_coefficients = other.m_coefficients;
            m_prime_value = other.m_prime_value;

            return *this;
        }

        int64_t at(int64_t value)
        {
            int64_t result = 0, mult = 1;

            for (auto el : m_coefficients)
            {
                result = (result + mult * el) % m_prime_value;
                mult = (mult * value) % m_prime_value;
            }

            return result % m_dom;
        }

    private:
        int64_t m_dom;
        int64_t m_prime_value;
        std::vector<int64_t> m_coefficients;
    };

    struct one_sparse_vector
    {
        explicit one_sparse_vector(int64_t size_, double delta_)
            : size(size_), prime_value(prime_more_than(4 * size_)),
            s_one(0), s_two(0),
            k_value(1 - 2 * int64_t(std::ceil(std::log2(delta_))))
        {
            std::uniform_int_distribution<int64_t> rand_int64_t(0, prime_value - 1);

            for (auto i = 0u; i < k_value; ++i)
            {
                tests.push_back(std::make_pair(0, rand_int64_t(mt)));
            }
        }

        one_sparse_vector(const one_sparse_vector & other)
        {
            size = other.size;
            prime_value = other.prime_value;
            k_value = other.k_value;
            s_one = other.s_one;
            s_two = other.s_two;
            tests = other.tests;
        }

        one_sparse_vector & operator=(const one_sparse_vector & other)
        {
            size = other.size;
            prime_value = other.prime_value;
            k_value = other.k_value;
            s_one = other.s_one;
            s_two = other.s_two;
            tests = other.tests;

            return *this;
        }

        one_sparse_vector copy()
        {
            one_sparse_vector result(size, 1.0);
            result.size = size;
            result.prime_value = prime_value;
            result.s_one = s_one;
            result.s_two = s_two;
            result.k_value = k_value;
            result.tests = {};
            for (auto & test : tests)
            {
                result.tests.push_back(test);
            }

            return result;
        }

        void update(int64_t index, int64_t value)
        {
            s_one += value;
            s_two += index * value;

            for (auto & test : tests)
            {
                test.first += (value * fast_pow(test.second, index, prime_value)) % prime_value;
            }
        }

        std::pair<int64_t, int64_t> recover()
        {
            return std::make_pair(s_two / s_one, s_one);
        }

        bool correct()
        {
            if (s_one == 0 || s_two % s_one != 0 || s_two * s_one < 0)
                return false;

            auto data = recover();

            for (auto & test : tests)
            {
                if ((data.second * fast_pow(test.second, data.first, prime_value)
                    - test.first) % prime_value != 0)
                {
                    return false;
                }
            }

            return true;
        }

        one_sparse_vector operator+(const one_sparse_vector & other)
        {
            one_sparse_vector result = copy();
            result.s_one = s_one + other.s_one;
            result.s_two = s_two + other.s_two;
            result.tests = {};
            
            for (int64_t i = 0; i < other.k_value; ++i)
            {
                int64_t first = (tests[i].first + other.tests[i].first) % other.prime_value;
                int64_t second = other.tests[i].second;

                result.tests.push_back(std::make_pair(first, second));
            }

            return result;
        }

        int64_t size;
        int64_t prime_value;
        int64_t s_one;
        int64_t s_two;
        int64_t k_value;
        std::vector< std::pair<int64_t, int64_t> > tests;
    };

    struct s_sparse_vector
    {
        explicit s_sparse_vector(int64_t size_, int64_t s_value_, double delta_)
            : cnt(0), size(size_), s_value(s_value_),
            k_value(1 - 2 * int64_t(std::ceil(std::log2(delta_ / 2.))))
        {
            double delta_decoder = delta_ / (2. * k_value * s_value);

            for (auto i = 0; i < k_value; ++i)
            {
                sketchs.push_back({});
                
                for (auto j = 0; j < 2 * s_value; ++j)
                {
                    sketchs.back().push_back(one_sparse_vector(size, delta_decoder));
                }

                hashes.push_back(hash_k(2 * s_value));
            }
        }

        s_sparse_vector(const s_sparse_vector & other)
        {
            size = other.size;
            s_value = other.s_value;
            cnt = other.cnt;
            k_value = other.k_value;
            sketchs = other.sketchs;
            hashes = other.hashes;
        }

        s_sparse_vector & operator=(const s_sparse_vector & other)
        {
            size = other.size;
            s_value = other.s_value;
            cnt = other.cnt;
            k_value = other.k_value;
            sketchs = other.sketchs;
            hashes = other.hashes;

            return *this;
        }

        s_sparse_vector copy()
        {
            s_sparse_vector result(size, 1, 1.0);
            result.cnt = cnt;
            result.size = size;
            result.s_value = s_value;
            result.k_value = k_value;
            result.sketchs = {};

            for (int64_t i = 0; i < k_value; ++i)
            {
                result.sketchs.push_back({});
                
                for (auto j = 0; j < 2 * s_value; ++j)
                {
                    result.sketchs.back().push_back(sketchs[i][j].copy());
                }
            }

            result.hashes = hashes;

            return result;
        }

        void update(int64_t index, int64_t value)
        {
            ++cnt;

            for (int64_t i = 0; i < k_value; ++i)
            {
                sketchs[i][hashes[i].at(index)].update(index, value);
            }
        }

        std::vector< std::pair<int64_t, int64_t> > recover()
        {
            std::map<int64_t, int64_t> tmp_r;
            std::vector< std::pair<int64_t, int64_t> > result;

            for (auto & table : sketchs)
            {
                for (auto & cell : table)
                {
                    if (cell.correct())
                    {
                        auto pair = cell.recover();

                        if (tmp_r.find(pair.first) == tmp_r.end())
                        {
                            result.push_back(pair);
                        }

                        tmp_r[pair.first] = pair.second;
                    }
                }
            }

            return result;
        }

        s_sparse_vector operator+(const s_sparse_vector & other)
        {
            s_sparse_vector result = copy();
            result.cnt += other.cnt;
            result.sketchs = {};
            
            for (int64_t i = 0; i < k_value; ++i)
            {
                result.sketchs.push_back({});
                
                for (auto j = 0; j < 2 * s_value; ++j)
                {
                    result.sketchs.back().push_back(sketchs[i][j] + other.sketchs[i][j]);
                }
            }

            return result;
        }

        bool touched()
        {
            return cnt != 0;
        }

        int64_t cnt;
        int64_t size;
        int64_t s_value;
        int64_t k_value;

        std::vector< std::vector<one_sparse_vector> > sketchs;
        std::vector< hash_k > hashes;
    };

    struct main_vector
    {
        main_vector()
            : s_value(0), k_value(0), size(0)
        {
        }

        explicit main_vector(int64_t size_, double delta_)
            : s_value(3 * (1 - int64_t(std::ceil(std::log2(delta_ / 2.))))),
            k_value(1 + int64_t(std::ceil(std::log(size_)))),
            size(size_), hash(hash_k(int64_t(std::pow(size_, 3)), s_value))
        {
            // std::cout << "S: " << s_value << "; k: " << k_value
            //             << "; size: " << size << "\n";

            double delta_decode = delta_ / 2.;

            for (auto i = 0; i < k_value; ++i)
            {
                sketchs.push_back(
                    s_sparse_vector(size, s_value, delta_decode));
            }
        }

        main_vector(const main_vector & other)
        {
            s_value = other.s_value;
            k_value = other.k_value;
            size = other.size;
            hash = other.hash;
            sketchs = other.sketchs;
        }

        main_vector & operator=(const main_vector & other)
        {
            s_value = other.s_value;
            k_value = other.k_value;
            size = other.size;
            hash = other.hash;
            sketchs = other.sketchs;

            return *this;
        }

        main_vector copy()
        {
            main_vector result(size, 1.0);
            result.s_value = s_value;
            result.k_value = k_value;
            result.size = size;
            result.hash = hash;
            result.sketchs = {};

            for (int64_t i = 0; i < k_value; ++i)
            {
                result.sketchs.push_back(sketchs[i].copy());
            }

            return result;
        }

        void update(int64_t index, int64_t value)
        {
            for (auto i = 0; i < k_value; ++i)
            {
                int64_t pow = int64_t(std::pow(2, i));
                if (hash.at(index) % pow == 0)
                {
                    sketchs[i].update(index, value);
                }
            }
        }

        std::pair<int64_t, int64_t> sample()
        {
            for (int64_t i = 0; i < k_value; ++i)
            {
                auto result = sketchs[k_value - 1 - i].recover();

                if (!result.empty())
                {
                    std::uniform_int_distribution<int64_t> rand_int64_t(0, result.size() - 1);

                    return result[rand_int64_t(mt)];
                }
            }

            return std::make_pair(0, 0);
        }

        main_vector operator+(const main_vector & other)
        {
            main_vector result = copy();
            result.sketchs = {};

            for (int64_t i = 0; i < k_value; ++i)
            {
                result.sketchs.push_back(sketchs[i] + other.sketchs[i]);
            }

            return result;
        }

        int64_t s_value;
        int64_t k_value;
        int64_t size;
        hash_k hash;
        std::vector< s_sparse_vector > sketchs;
    };
}

class dsu
{
public:
    explicit dsu(int64_t vertex_count)
        : m_vertex_count(vertex_count)
    {
        for (int64_t i = 0; i < m_vertex_count; ++i)
        {
            m_rank.push_back(0);
            m_parent.push_back(i);
        }
    }

    int64_t find(int64_t u)
    {
        if (m_parent.at(u) == u)
        {
            return u;
        }

        m_parent[u] = find(m_parent.at(u));

        return m_parent.at(u);
    }

    void union_(int64_t u, int64_t v)
    {
        u = find(u);
        v = find(v);

        if (m_rank.at(u) > m_rank.at(v))
        {
            std::swap(u, v);
        }

        if (m_rank.at(u) == m_rank.at(v))
        {
            ++m_rank[v];
        }

        m_parent[u] = v;
    }

private:
    int64_t m_vertex_count;
    std::vector<int64_t> m_rank;
    std::vector<int64_t> m_parent;
};

constexpr double delta_const = 0.01;

class DynamicGraph
{
public:
    explicit DynamicGraph(int64_t vertex_count)
        : m_vertex_count(vertex_count),
        m_sketch_count(1 + int64_t(std::ceil(std::log2(vertex_count))))
    {
        // std::cout << "DynamicGraph: " << "vertex count: " << m_vertex_count
        //             << "; k: " << m_sketch_count << "\n";

        int64_t pow = int64_t(std::pow(m_vertex_count, 2));
        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch.push_back(generate_graph_sketch(
                                    m_vertex_count, pow, delta_const));
        }
    }

    void AddEdge(int64_t u, int64_t v)
    {
        if (u > v) std::swap(u, v);

        int64_t edge_number = u * m_vertex_count + v;

        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch[i][u].update(edge_number, +1);
            m_sketch[i][v].update(edge_number, -1);
        }
    }

    void RemoveEdge(int64_t u, int64_t v)
    {
        if (u > v) std::swap(u, v);

        int64_t edge_number = u * m_vertex_count + v;

        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch[i][u].update(edge_number, -1);
            m_sketch[i][v].update(edge_number, +1);
        }
    }

    int64_t GetComponentsNumber() const
    {
        std::map< int64_t, std::vector<int64_t> > cur_cc;

        for (int64_t i = 0; i < m_vertex_count; ++i)
        {
            cur_cc[i] = { i };
        }

        dsu _dsu(m_vertex_count);

        for (int64_t lev = 0; lev < m_sketch_count; ++lev)
        {
            std::map<int64_t, l0sample::main_vector> sketch_sum;

            for (auto it = cur_cc.begin(); it != cur_cc.end(); ++it)
            {
                const int64_t & key = it->first;
                auto & component = it->second;

                sketch_sum[key] = m_sketch[lev][component[0]];

                for (uint64_t j = 1; j < component.size(); ++j)
                {
                    sketch_sum[key] = sketch_sum[key] + m_sketch[lev][component[j]];
                }
            }

            for (auto it = sketch_sum.begin(); it != sketch_sum.end(); ++it)
            {
                auto pair = it->second.sample();

                if (pair.first != 0 && pair.second != 0)
                {
                    int64_t u_ = pair.first / m_vertex_count;
                    int64_t v_ = pair.first % m_vertex_count;

                    // std::cout << "(" << u_ << ", " << v_ << ", " 
                    //             << pair.first << ", " << m_vertex_count << ")\n";

                    _dsu.union_(u_, v_);
                }
            }

            cur_cc.clear();

            for (auto i = 0; i < m_vertex_count; ++i)
            {
                auto parent = _dsu.find(i);

                cur_cc[parent].push_back(i);
            }
        }

        return cur_cc.size();
    }

private:
    std::vector< l0sample::main_vector >
        generate_graph_sketch(int64_t size, int64_t msize, double delta)
    {
        l0sample::main_vector sketch(msize, delta);

        std::vector< l0sample::main_vector > result;

        for (auto i = 0; i < size; ++i)
        {
            result.push_back(sketch.copy());
        }

        return result;
    }

private:
    const int64_t m_vertex_count;
    const int64_t m_sketch_count;

    std::vector< std::vector<l0sample::main_vector> > m_sketch;
};
