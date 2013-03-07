#ifndef PROXY_HPP_MODULE
#define PROXY_HPP_MODULE

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/logger.h>

#include <boost/tokenizer.hpp>
#include <boost/shared_ptr.hpp>

#include <elliptics/proxy.hpp>

#include <map>

class Proxy : 
	  virtual public fastcgi::Component
	, virtual public fastcgi::Handler
{
private:
	typedef void (Proxy::*RequestHandler) (fastcgi::Request *request);
	//using RequestHandler = void (Proxy::*) (fastcgi::Request *request);
	typedef std::map <std::string, RequestHandler> RequestHandlers;	

public:
    typedef boost::char_separator <char> Separator;
    typedef boost::tokenizer <Separator> Tokenizer;

	Proxy (fastcgi::ComponentContext *context);
	virtual ~Proxy ();

	virtual void onLoad ();
	virtual void onUnload ();
	virtual void handleRequest (fastcgi::Request *request, fastcgi::HandlerContext *context);

private:
	fastcgi::Logger *log () const;

    //size_t paramsNum (Tokenizer &tok) const;
    //void dnet_parse_numeric_id (const std::string &value, struct dnet_id &id) const;
    //void getGroups (fastcgi::Request *request, std::vector <int> &groups, int count = 0) const;
    //elliptics::Key getKey (fastcgi::Request *request) const;

	void registerHandler (const char *name, RequestHandler handler);

	void uploadHandler (fastcgi::Request *request);
	void getHandler (fastcgi::Request *request);
	void deleteHandler (fastcgi::Request *request);
	void downloadInfoHandler (fastcgi::Request *request);

	fastcgi::Logger *logger_;
    elliptics::EllipticsProxy::config elconf;
	boost::shared_ptr  <elliptics::EllipticsProxy> ellipticsProxy_;

	RequestHandlers handlers_;

	int write_port_;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("proxy_factory", Proxy)
FCGIDAEMON_REGISTER_FACTORIES_END()

#endif /* PROXY_HPP_MODULE */
