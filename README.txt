1. All listeners connectors and services are singleton classes;
2. rename all file names with '_', and some variable names according to Google Coding Standard;
3. add Price entry to Trade class;
4. implemented Connector.Publish function, instead of a pure virtual function. Because some derived connector does not need Publish;
5. Replace T in template with Bond, because we only trade Bond.
6. Move functions of non-template classes into *.cpp file. 
7. Move template function implementations into its own class, for the sake of including simplicity; 
8. Added init_positions function in Position class.
9. I assume, BUY means, customer buy from us, therefore the amount in our trade book would decrease;

