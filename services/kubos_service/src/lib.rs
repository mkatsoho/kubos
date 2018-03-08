//
// Copyright (C) 2017 Kubos Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#[macro_use]
extern crate iron;
extern crate mount;
extern crate logger;
extern crate juniper_iron;
extern crate juniper;

use iron::prelude::*;
use juniper_iron::{GraphQLHandler, GraphiQLHandler};
use juniper::{GraphQLType, RootNode};

use std::env;

pub struct Service<CtxFactory, Query, Mutation, CtxT>
where
    CtxFactory: Fn(&mut Request) -> CtxT + Send + Sync + 'static,
    CtxT: 'static,
    Query: GraphQLType<Context = CtxT> + Send + Sync + 'static,
    Mutation: GraphQLType<Context = CtxT> + Send + Sync + 'static,
{
    context_factory: CtxFactory,
    query: Query,
    mutation: Mutation,
    port: String
}

impl<CtxFactory, Query, Mutation, CtxT> Service<CtxFactory, Query, Mutation, CtxT>
where
    CtxFactory: Fn(&mut Request) -> CtxT + Send + Sync + 'static,
    CtxT: 'static,
    Query: GraphQLType<Context = CtxT, TypeInfo = ()> + Send + Sync + 'static,
    Mutation: GraphQLType<Context = CtxT, TypeInfo = ()> + Send + Sync + 'static,
{
    /// Build a new GraphQL Service handler
    ///
    /// The context factory will receive the Iron request object and is
    /// expected to construct a context object for the given schema. This can
    /// be used to construct e.g. database connections or similar data that
    /// the schema needs to execute the query.
    pub fn new(context_factory: CtxFactory, query: Query, mutation: Mutation, port: String) -> Self {
        Service {
            context_factory,
            query,
            mutation,
            port
        }
    }

    pub fn start(&self, context_factory: CtxFactory, query: Query, mutation: Mutation) {
        let graphql_endpoint = GraphQLHandler::new(
            context_factory,
            query,
            mutation);

        let graphiql_endpoint = GraphiQLHandler::new("/graphiql");

        let mut mount = mount::Mount::new();
        // mount.mount("/", graphql_endpoint);
        mount.mount("/graphiql", graphiql_endpoint);

        let (logger_before, logger_after) = logger::Logger::new(None);

        let mut chain = Chain::new(mount);
        chain.link_before(logger_before);
        chain.link_after(logger_after);

        let ipport = format!("0.0.0.0:{}", self.port);

        let host = env::var("LISTEN").unwrap_or(ipport.to_owned());
        println!("GraphQL server started on {}", host);
        Iron::new(chain).http(host.as_str()).unwrap();
    }
}
