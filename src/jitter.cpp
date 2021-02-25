#include <libgccjit.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <map>
#include <string>
#include <vector>

class FunctionRegistry {
    public:
        FunctionRegistry() {

        }

        void add_function(std::string name, gcc_jit_function *function) {
            this->methods[name] = function;
        }

        gcc_jit_function *get_function(std::string name) {
            return this->methods[name];
        }

        ~FunctionRegistry() {

        }
    protected:
    private:
        std::map<std::string, gcc_jit_function *> methods;
};

class JitterMethod {
    public:
        JitterMethod(FunctionRegistry *function_registry, gcc_jit_context *ctx, std::string name) {
            this->function_registry = function_registry;
            this->ctx = ctx;
            this->name = name;
        }

        ~JitterMethod() {
        }

        void add_parameter(const char *name, gcc_jit_type *type) {
            this->parameters.push_back(
                gcc_jit_context_new_param(
                    this->ctx,
                    NULL,
                    type,
                    name
                )
            );
        }

        void build(gcc_jit_type *return_type) {
            this->func = gcc_jit_context_new_function(
                this->ctx,
                NULL,
                // This is always exported? I think so.
                GCC_JIT_FUNCTION_EXPORTED,
                return_type,
                this->name.c_str(),
                this->parameters.size(),
                this->parameters.data(),
                0
            );

            gcc_jit_rvalue *args[2];
            args[0] = gcc_jit_context_new_string_literal(this->ctx, "hello %s\n");
            args[1] = gcc_jit_param_as_rvalue(this->parameters.at(0));

            gcc_jit_block *block = gcc_jit_function_new_block(this->func, NULL);
            gcc_jit_block_add_eval(
                block,
                NULL,
                gcc_jit_context_new_call(
                    this->ctx,
                    NULL,
                    this->function_registry->get_function("printf"),
                    2,
                    args
                )
            );

            gcc_jit_block_end_with_void_return(block, NULL);
        }
    protected:
    private:
        gcc_jit_context *ctx;
        gcc_jit_function *func;
        
        std::string name;

        FunctionRegistry *function_registry;

        std::vector<gcc_jit_param *> parameters;
};

class Jitter {
    public:
        Jitter() {
            this->ctx = gcc_jit_context_acquire();

            gcc_jit_context_set_bool_option(
                this->ctx,
                GCC_JIT_BOOL_OPTION_DUMP_GENERATED_CODE,
                true
            );

            this->const_char_ptr_type = gcc_jit_context_get_type(this->ctx, GCC_JIT_TYPE_CONST_CHAR_PTR);
            this->int_type = gcc_jit_context_get_type(this->ctx, GCC_JIT_TYPE_INT);
            this->void_type = gcc_jit_context_get_type(this->ctx, GCC_JIT_TYPE_VOID);

            gcc_jit_param *param_format = gcc_jit_context_new_param(
                this->ctx,
                NULL,
                this->const_char_ptr_type,
                "format"
            );
            this->function_registry.add_function(
                "printf",
                gcc_jit_context_new_function(
                    this->ctx,
                    NULL,
                    GCC_JIT_FUNCTION_IMPORTED,
                    this->int_type,
                    "printf",
                    1,
                    &param_format,
                    1
                )
            );
        }

        gcc_jit_type *get_const_char_ptr_type() {
            return this->const_char_ptr_type;
        }

        gcc_jit_type *get_void_type() {
            return this->void_type;
        }

        JitterMethod *start_method(const char *name) {
            return new JitterMethod(&this->function_registry, this->ctx, name);
        }

        void finish() {
            gcc_jit_result *result;

            /* Compile the code.  */
            result = gcc_jit_context_compile(this->ctx);
            if (!result) {
                fprintf(stderr, "NULL result");
                exit(1);
            }

            /* Extract the generated code from "result".  */
            typedef void (*fn_type) (const char *);
            fn_type greet = (fn_type)gcc_jit_result_get_code(result, "greet");
            if (!greet) {
                fprintf(stderr, "NULL greet");
                exit(1);
            }
            
            /* Now call the generated function: */
            greet("world");
            fflush(stdout);

            // This is done after context release in the examples.
            gcc_jit_result_release(result);
        }

        ~Jitter() {
            gcc_jit_context_release(this->ctx);
        }
    protected:
    private:
        gcc_jit_context *ctx;

        gcc_jit_type *const_char_ptr_type;
        gcc_jit_type *int_type;
        gcc_jit_type *void_type;

        FunctionRegistry function_registry;
};

int main (int argc, char **argv) {
    Jitter jitter;

    JitterMethod *method = jitter.start_method("greet");

    method->add_parameter("name", jitter.get_const_char_ptr_type());
    method->build(jitter.get_void_type());

    jitter.finish();

    delete method;

    return 0;
}