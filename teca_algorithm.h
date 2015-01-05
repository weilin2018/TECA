#ifndef teca_algorithm_h
#define teca_algorithm_h

#include <vector>
#include <memory>
#include <utility>
#include <iosfwd>

#include "teca_dataset.h"
#include "teca_meta_data.h"

class teca_algorithm;
class teca_algorithm_internals;
typedef std::shared_ptr<teca_algorithm> p_teca_algorithm;

// interface to teca pipeline architecture. all sources/readers
// filters, sinks/writers will implement this interface
class teca_algorithm : public std::enable_shared_from_this<teca_algorithm>
{
public:
    // convinience method for creating the algorithm
    // in a shared pointer.
    static p_teca_algorithm New();
    ~teca_algorithm();

    // convenience method tp get an output port from
    // the algorithm to be used during pipeline building
    typedef std::pair<p_teca_algorithm, unsigned int> output_port_t;
    output_port_t get_output_port(unsigned int port);

    // set an input to this algorithm
    void set_input_connection(
        unsigned int id,
        const output_port_t &port);

    void remove_input_connection(unsigned int id);
    void clear_input_connections();

    // access the data produced by this algorithm.
    p_teca_dataset get_output_data(unsigned int port);

    // execute the pipeline from this instance up.
    virtual int update();

protected:
    teca_algorithm();

    teca_algorithm(const teca_algorithm &src);
    teca_algorithm(teca_algorithm &&src);

    teca_algorithm &operator=(const teca_algorithm &src);
    teca_algorithm &operator=(teca_algorithm &&src);

    // implementations should call this from their
    // constructors to setup the internal caches
    // and data structures required for execution.
    void set_number_of_inputs(unsigned int n);
    void set_number_of_outputs(unsigned int n);

    // set the modified flag on the given output
    // port's cache. should be called when user
    // modifies properties on the object
    // that require the output to be regenerated.
    void set_modified();
    void set_modified(unsigned int port);

    p_teca_dataset get_output_data(
        unsigned int port,
        const teca_meta_data &request);

private:
    // implementations must override this method to provide
    // information to downstream consumers about what data
    // will be produced on each output port. The port to
    // provide information about is named in the first argument
    // the second argument contains a list of the metadata
    // describing data on all of the inputs.
    virtual
    teca_meta_data get_output_meta_data(
        unsigned int port,
        std::vector<teca_meta_data> &input_md);

    // implementations must override this method and
    // generate a set of requests describing the data
    // required on the inputs to produce data for the
    // named output port, given the upstream meta data
    // and request. If no data is needed on an input
    // then the list should contain a null request.
    virtual
    std::vector<teca_meta_data> get_upstream_request(
        unsigned int port,
        std::vector<teca_meta_data> &input_md,
        teca_meta_data &request);

    // implementations must override this method and
    // produce the output dataset for the port named
    // in the first argument. The second argument is
    // a list of all of the input datasets. See also
    // get_request. The third argument contains a request
    // from the consumer which can spcify information
    // such as arrays, subset region, timestep etc.
    // The implementation is free to handle the request
    // as it sees fit.
    virtual
    p_teca_dataset execute(
        unsigned int port,
        std::vector<p_teca_dataset> &input_data,
        teca_meta_data &request);

    // manage meta data reporting  phase of pipeline
    // execution.
    teca_meta_data get_output_meta_data(
        output_port_t &current);

    // manage execution of the given requst on the
    // named port
    p_teca_dataset request_data(
        output_port_t &port,
        teca_meta_data &request);

    // clear chache where modified flag has been set
    int validate_cache(output_port_t &current);

    // clear the modified flag after execution
    void clear_modified(output_port_t &current);

    // serialize the configuration to a stream
    virtual void to_stream(std::ostream &os);
    virtual void from_stream(std::istream &is);

private:
    teca_algorithm_internals *internals;
};

#endif
