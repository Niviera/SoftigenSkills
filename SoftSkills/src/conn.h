class connection
{
private:
    /* data */
    const char *WIFI_NETWORK = "Homesurfing";
    const char *WIFI_PASSWORD = "WRFGJLM6zf7AjrWkoeXfaWwdKYuYuWaR";

    const int timeoutMS = 20000;
public:
    connection(/* args */);
    ~connection();

    bool connectToWifi();
};
