#include <iostream>
#include <string>
#include <functional>
#include <tuple>

#include "DynamicGraph.hpp"

// test rbtree
void tests_rbtree();

// tests l0sample
void tests_fast_pow();
void tests_one_sparse_vector();
void tests_s_sparse_vector();
void tests_main_vector();

// tests DynamicGraph
void tests_dynamic_graph();
void speed_test();

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // tests rbtree
    //tests_rbtree();

    // tests l0sample
    tests_fast_pow();
    tests_one_sparse_vector();
    tests_s_sparse_vector();
    tests_main_vector();

    // tests DynamicGraph
    tests_dynamic_graph();

    system("pause");

    return 0;
}

void tests_rbtree()
{
    std::cout << "Tests rbtree\n";

    // Test 1
    {
        std::cout << "Test 1:\n";

        rbtree::tree<uint32_t, int32_t> dict;

        for (auto i = 0u; i < 10; ++i)
        {
            dict.push(i, i + 1);
        }

        for (auto * el : dict.items)
        {
            std::cout << "(" << el->info.first << ", " << el->info.second << ") ";
        }
        std::cout << "\n";

        dict.push(5, -100);
        dict.push(9, -100000);

        for (auto * el : dict.items)
        {
            std::cout << "(" << el->info.first << ", " << el->info.second << ") ";
        }
        std::cout << "\n";
    }

    // Test 2
    {
        std::cout << "Test 2:\n";

        rbtree::tree<uint32_t, int32_t> dict;

        for (auto i = 0u; i < 10; ++i)
        {
            dict.push(i, i + 1);
        }

        for (auto * el : dict.items)
        {
            std::cout << "(" << el->info.first << ", " << el->info.second << ") ";
        }
        std::cout << "\n";

        dict.clear();

        for (auto i = 0u; i < 3; ++i)
        {
            dict.push(3, 21);
        }

        for (auto * el : dict.items)
        {
            std::cout << "(" << el->info.first << ", " << el->info.second << ") ";
        }
        std::cout << "\n";
    }

    // Test 3
    {
        std::cout << "Test 3:\n";

        rbtree::tree<uint32_t, int32_t> dict;

        for (auto i = 0u; i < 10; ++i)
        {
            dict.push(i, i + 1);
        }

        dict.clear();

        auto * result = dict.find(5);

        if (result)
        {
            std::cout << "result: (" << result->info.first << ", " << result->info.second << ")\n";
        }
        else
        {
            std::cout << "Not found\n";
        }
    }

    // Test 4
    {
        std::cout << "Test 4:\n";

        rbtree::tree<uint32_t, int32_t> dict;

        for (auto i = 0u; i < 5; ++i)
        {
            dict.push(i, 10 + i);
        }

        for (auto i = 0u; i < 6; ++i)
        {
            auto & el = dict[i];

            std::cout << i << ": " << el << "\n";
        }

        //rbtree::tree<uint32_t, std::vector<int32_t>> dict;

        //for (auto i = 0u; i < 5; ++i)
        //{
        //    std::vector<int32_t> tmp;
        //    for (auto j = 10; j < 13; ++j)
        //    {
        //        tmp.push_back(j);
        //    }

        //    dict.push(i, tmp);
        //}
        //
        ////dict[5] = { -1000 };
        ////dict[1] = { 3 };
        ////dict[10] = { 1, 2, 3 };

        //for (auto i = 0u; i < 6; ++i)
        //{
        //    if (i == 5)
        //    {
        //    }

        //    auto & tmp = dict[i];
        //    std::cout << i << ": [ ";
        //    for (auto & el : tmp)
        //    {
        //        std::cout << el << ", ";
        //    }
        //    std::cout << "]\n";
        //}
    }
}

void tests_fast_pow()
{
    std::cout << "Tests function fast_pow:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        if (l0sample::fast_pow(1469, 17, 1601) == 1411709)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        if (l0sample::fast_pow(1191, 17, 1601) == 1884162)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        if (l0sample::fast_pow(301, 17, 1601) == 87290)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        if (l0sample::fast_pow(20000, 17, 1601) == 19700000)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }
}

void tests_one_sparse_vector()
{
    std::cout << "Tests one sparse vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::one_sparse_vector r(100, 0.001);

        if (r.correct() != false)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        l0sample::one_sparse_vector r(100, 0.001);
        r.update(17, 42);

        if (r.correct() != true)
        {
            std::cout << "Fail 1\n";
            return;
        }

        auto pair = r.recover();

        if (pair.first != 17 || pair.second != 42)
        {
            std::cout << "Fail 2\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        l0sample::one_sparse_vector r(100, 0.001);
        r.update(1, 1);
        r.update(2, 2);

        if (r.correct() != false)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        l0sample::one_sparse_vector r(100, 0.001);

        for (auto i = 0u; i < 100; ++i)
        {
            r.update(i, i + 17);
        }
        r.update(17, 42);

        for (auto i = 0u; i < 100; ++i)
        {
            r.update(i, -int32_t(i + 17));
        }

        if (r.correct() != true)
        {
            std::cout << "Fail 1\n";
            return;
        }

        auto pair = r.recover();

        if (pair.first != 17 || pair.second != 42)
        {
            std::cout << "Fail 2\n";
            return;
        }

        std::cout << "True\n";
    }
}

void tests_s_sparse_vector()
{
    std::cout << "Tests s sparse vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        if (r.touched() != false)
        {
            std::cout << "False\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        if (!r.recover().empty())
        {
            std::cout << "False\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        r.update(17, 42);

        std::uniform_int_distribution<uint32_t> rand_uint32_t_one(0, 99);
        std::uniform_int_distribution<int32_t> rand_uint32_t_two(1, 25);

        for (auto i = 0u; i < 100; ++i)
        {
            auto ind = rand_uint32_t_one(mt);
            auto val = rand_uint32_t_two(mt);

            r.update(ind, val);
            r.update(ind, -val);
        }

        if (r.touched() != true)
        {
            std::cout << "False 1\n";
            return;
        }

        auto result = r.recover();

        if (result.size() != 1)
        {
            std::cout << "False 2\n";
            return;
        }

        if (result[0].first != 17 || result[0].second != 42)
        {
            std::cout << "False 3\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        std::vector< std::pair<uint32_t, int32_t> > updates = {
            { 17, 42 },{ 18, 43 },{ 19, 44 },{ 20, 45 },{ 21, 46 }
        };

        for (auto & pair : updates)
        {
            r.update(pair.first, pair.second);
        }

        std::uniform_int_distribution<uint32_t> rand_uint32_t_one(0, 99);
        std::uniform_int_distribution<int32_t> rand_uint32_t_two(1, 25);

        for (auto i = 0u; i < 100; ++i)
        {
            auto ind = rand_uint32_t_one(mt);
            auto val = rand_uint32_t_two(mt);

            r.update(ind, val);
            r.update(ind, -val);
        }

        auto result = r.recover();

        if (result.size() != updates.size())
        {
            std::cout << "False 1\n";
            return;
        }

        for (auto & pair : updates)
        {
            auto search = std::find(result.begin(), result.end(), pair);
            if (search == result.end())
            {
                std::cout << "Fail 2\n";
                return;
            }
        }

        std::cout << "True\n";
    }
}

void tests_main_vector()
{
    std::cout << "Tests main vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::main_vector r(100, 0.01);

        auto result = r.sample();

        if (result.first != 0 || result.second != 0)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        uint32_t size = 20;

        std::vector< l0sample::main_vector > rs;

        for (auto i = 0u; i < 10; ++i)
        {
            rs.push_back(l0sample::main_vector(size, 0.01));
        }

        std::vector<int32_t> vals(size, 0);

        std::uniform_int_distribution<uint32_t> rand_uint32_t_one(0, size - 1);
        std::uniform_int_distribution<int32_t> rand_int32_t_two(-5, 5);

        for (auto i = 0; i < size; ++i)
        {
            auto ind = rand_uint32_t_one(mt);
            auto val = rand_int32_t_two(mt);

            vals[ind] += val;

            for (auto & r : rs)
            {
                r.update(ind, val);
            }
        }

        for (auto i = 0u; i < size; ++i)
        {
            if (i % 2 == 1 && vals[i] != 0)
            {
                for (auto & r : rs)
                {
                    r.update(i, -vals[i]);
                }

                vals[i] -= vals[i];
            }
        }

        std::vector< std::pair<uint32_t, int32_t> > answers;

        //std::cout << "vals:\n";
        //for (auto i = 0u; i < vals.size(); ++i)
        //{
        //    std::cout << i << "  ";
        //}
        //std::cout << "\n";
        //for (auto i = 0u; i < vals.size(); ++i)
        //{
        //    std::cout << vals[i] << ", " << std::string(i + 1 > 9 ? 1 : 0, ' ');
        //}


        //std::cout << "\nanswer:\n";
        for (auto & r : rs)
        {
            answers.push_back(r.sample());
            //std::cout << "(" << (answers.end() - 1)->first << ", "
            //    << (answers.end() - 1)->second << "), ";
        }

        //std::cout << "\n";

        for (auto a : answers)
        {
            if (a.first % 2 != 0 || a.second != vals[a.first])
            {
                std::cout << "False\n";
                return;
            }
        }

        std::cout << "True\n";
    }
}

void tests_dynamic_graph()
{
    std::cout << "Tests dynamic graph:\n";

    // Test 1
    {
        std::cout << "Test 1:\n";

        DynamicGraph g(6);

        std::vector< std::pair<uint32_t, uint32_t> > edges = {
            { 0, 1 },{ 1, 2 },{ 1, 3 },{ 2, 3 },{ 4, 5 }
        };

        for (auto & pair : edges)
        {
            g.AddEdge(pair.first, pair.second);
        }

        auto result = g.GetComponentsNumber();

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "Test 2:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(uint32_t, uint32_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);

        std::vector<std::tuple<uint32_t, uint32_t, decltype(add)>> edges = {
            { 4, 5, add },{ 0, 1, add },{ 1, 3, add },{ 2, 3, add },{ 1, 2, add }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        std::cout << "Size: " << result << "\n";

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "Test 3:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(uint32_t, uint32_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(uint32_t, uint32_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<uint32_t, uint32_t, decltype(add)>> edges = {
            { 0, 2, add },{ 0, 1, add },{ 1, 2, add },{ 0, 3, rem },
        { 1, 3, add },{ 2, 3, add },{ 1, 5, add },{ 4, 5, add },
        { 1, 5, rem }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "Test 4:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(uint32_t, uint32_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(uint32_t, uint32_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<uint32_t, uint32_t, decltype(add)>> edges = {
            { 0, 2, add },{ 0, 1, add },{ 1, 2, add },{ 0, 2, rem },
        { 1, 3, add },{ 2, 3, add },{ 1, 5, add },{ 4, 5, add },
        { 1, 5, rem },{ 0, 1, rem },{ 1, 2, rem },{ 4, 5, rem },
        { 1, 3, rem },{ 2, 3, rem }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        if (result != 6)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }
}

void speed_test()
{
    std::cout << "Speed test:\n";
    DynamicGraph g(6);

    using std::placeholders::_1;
    using std::placeholders::_2;

    std::function<void(uint32_t, uint32_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
    std::function<void(uint32_t, uint32_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

    std::vector<std::tuple<uint32_t, uint32_t, decltype(add)>> edges = {
        { 0, 2, add },{ 0, 1, add },{ 1, 2, add },{ 0, 2, rem },
    { 1, 3, add },{ 2, 3, add },{ 1, 5, add },{ 4, 5, add },
    { 1, 5, rem },{ 0, 1, rem },{ 1, 2, rem },{ 4, 5, rem },
    { 1, 3, rem },{ 2, 3, rem }
    };

    for (auto & trio : edges)
    {
        auto u = std::get<0>(trio);
        auto v = std::get<1>(trio);
        auto comd = std::get<2>(trio);

        comd(u, v);
    }

    auto result = g.GetComponentsNumber();

    if (result != 6)
    {
        std::cout << "Fail\n";
        return;
    }

    std::cout << "True\n";
}

