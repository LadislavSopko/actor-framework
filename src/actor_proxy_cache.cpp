#include "cppa/atom.hpp"
#include "cppa/message.hpp"
#include "cppa/detail/thread.hpp"
#include "cppa/detail/actor_proxy_cache.hpp"

// thread_specific_ptr
#include <boost/thread.hpp>

namespace cppa { namespace detail {

class actor_proxy_caches {

public:

    actor_proxy_cache& get_actor_proxy_cache()
    {
        if (m_proxy_cache.get() == nullptr)
        {
            m_proxy_cache.reset(new actor_proxy_cache);
        }
        return *m_proxy_cache.get();
    }

    static actor_proxy_caches& get()
    {
        return *s_instance;
    }

private:

    static actor_proxy_caches* s_instance;

    boost::thread_specific_ptr<cppa::detail::actor_proxy_cache> m_proxy_cache;

};

// TODO: free
actor_proxy_caches* actor_proxy_caches::s_instance = new actor_proxy_caches();

process_information_ptr
actor_proxy_cache::get_pinfo(const actor_proxy_cache::key_tuple& key)
{
    auto i = m_pinfos.find(key);
    if (i != m_pinfos.end())
    {
        return i->second;
    }
    process_information_ptr tmp(new process_information(std::get<1>(key),
                                                        std::get<2>(key)));
    m_pinfos.insert(std::make_pair(key, tmp));
    return tmp;
}

actor_proxy_ptr actor_proxy_cache::get(const key_tuple& key)
{
    auto i = m_proxies.find(key);
    if (i != m_proxies.end())
    {
        return i->second;
    }
    // get_pinfo(key) also inserts to m_pinfos
    actor_proxy_ptr result(new actor_proxy(std::get<0>(key), get_pinfo(key)));
    m_proxies.insert(std::make_pair(key, result));
    if (m_new_cb) m_new_cb(result);
    // insert to m_proxies
    //result->enqueue(message(result, nullptr, atom(":Monitor")));
    return result;
}

void actor_proxy_cache::add(actor_proxy_ptr& pptr)
{
    auto pinfo = pptr->parent_process_ptr();
    key_tuple key(pptr->id(), pinfo->process_id(), pinfo->node_id());
    m_pinfos.insert(std::make_pair(key, pptr->parent_process_ptr()));
    m_proxies.insert(std::make_pair(key, pptr));
    if (m_new_cb) m_new_cb(pptr);
}

void actor_proxy_cache::erase(const actor_proxy_ptr& pptr)
{
    auto pinfo = pptr->parent_process_ptr();
    key_tuple key(pptr->id(), pinfo->process_id(), pinfo->node_id());
    m_proxies.erase(key);
}

size_t actor_proxy_cache::size() const
{
    return m_proxies.size();
}

actor_proxy_cache& get_actor_proxy_cache()
{
    return actor_proxy_caches::get().get_actor_proxy_cache();
}

} } // namespace cppa::detail
