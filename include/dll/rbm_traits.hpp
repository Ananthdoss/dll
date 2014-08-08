//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef DLL_RBM_TRAITS_HPP
#define DLL_RBM_TRAITS_HPP

#include "tmp.hpp"
#include "decay_type.hpp"

namespace dll {

template<typename Desc>
class dyn_rbm;

template<typename Desc>
class conv_rbm;

template<typename Desc>
class conv_rbm_mp;

/*!
 * \brief Type Traits to get information on RBM type
 */
template<typename RBM>
struct rbm_traits {
    using rbm_t = RBM;

    HAS_STATIC_FIELD(BatchSize, has_batch_size_field)
    HAS_STATIC_FIELD(Sparsity, has_sparsity_field)
    HAS_STATIC_FIELD(Decay, has_decay_field)
    HAS_STATIC_FIELD(Init, has_init_field)
    HAS_STATIC_FIELD(Momentum, has_momentum_field)

    /*!
     * \brief Indicates if the RBM is convolutional
     */
    static constexpr bool is_convolutional(){
        return detail::is_instantiation_of<conv_rbm, rbm_t>::value
            || detail::is_instantiation_of<conv_rbm_mp, rbm_t>::value;
    }

    /*!
     * \brief Indicates if the RBM is dynamic
     */
    static constexpr bool is_dynamic(){
        return detail::is_instantiation_of<dyn_rbm, rbm_t>::value;
    }

    /*!
     * \brief Indicates if the RBM is convolutional and has probabilistic max
     * pooling
     */
    static constexpr bool has_probabilistic_max_pooling(){
        return detail::is_instantiation_of<conv_rbm_mp, rbm_t>::value;
    }

    template<typename R = RBM, enable_if_u<has_batch_size_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr std::size_t batch_size(){
        return rbm_t::desc::BatchSize;
    }

    template<typename R = RBM, disable_if_u<has_batch_size_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr std::size_t batch_size(){
        return 1;
    }

    template<typename R = RBM, enable_if_u<has_momentum_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool has_momentum(){
        return rbm_t::desc::Momentum;
    }

    template<typename R = RBM, disable_if_u<has_momentum_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool has_momentum(){
        return false;
    }

    template<typename R = RBM, enable_if_u<has_sparsity_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool has_sparsity(){
        return rbm_t::desc::Sparsity;
    }

    template<typename R = RBM, disable_if_u<has_sparsity_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool has_sparsity(){
        return false;
    }

    template<typename R = RBM, enable_if_u<has_decay_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr decay_type decay(){
        return rbm_t::desc::Decay;
    }

    template<typename R = RBM, disable_if_u<has_decay_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr decay_type decay(){
        return decay_type::NONE;
    }

    template<typename R = RBM, enable_if_u<has_init_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool init_weights(){
        return rbm_t::desc::Init;
    }

    template<typename R = RBM, disable_if_u<has_init_field<typename R::desc>::value> = ::detail::dummy>
    static constexpr bool init_weights(){
        return false;
    }
};

template<typename RBM, enable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
std::size_t get_batch_size(const RBM& rbm){
    return rbm.batch_size;
}

template<typename RBM, disable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
constexpr std::size_t get_batch_size(const RBM&){
    return rbm_traits<RBM>::batch_size();
}

template<typename RBM, enable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
std::size_t num_visible(const RBM& rbm){
    return rbm.num_visible;
}

template<typename RBM, disable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
constexpr std::size_t num_visible(const RBM&){
    return RBM::desc::num_visible;
}

template<typename RBM, enable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
std::size_t num_hidden(const RBM& rbm){
    return rbm.num_hidden;
}

template<typename RBM, disable_if_u<rbm_traits<RBM>::is_dynamic()> = ::detail::dummy>
constexpr std::size_t num_hidden(const RBM&){
    return RBM::desc::num_hidden;
}

} //end of dbn namespace

#endif