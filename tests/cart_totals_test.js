Feature('Cart totals');

Scenario('Bad cart', async ({I}) => {
    var resp = await I.sendPostRequest('/V1/guest-carts/bad_cart/totals-information',
        {
            "addressInformation": {
                "address": {
                    "countryId": "US",
                    "region": "Florida",
                    "regionId": "18",
                    "postcode": null,
                    "city": "",
                    "extension_attributes": {
                        "advanced_conditions": {
                            "payment_method": null,
                            "city": "",
                            "shipping_address_line": [],
                            "billing_address_country": null,
                            "currency": "USD"
                        }
                    }
                }, "shipping_method_code": "freeshipping", "shipping_carrier_code": "freeshipping"
            }
        });

    I.assertEqual(resp.status, 404);
    I.assertEqual(resp.data.message, "No such entity with %fieldName = %fieldValue");
    I.assertEqual(resp.data.parameters.fieldName, "cartId");
    I.assertEqual(resp.data.parameters.fieldValue, null);
});

Scenario('Good cart', async ({I}) => {

    var resp = await I.sendPostRequest('/V1/guest-carts/good_cart/totals-information',
        {
            "addressInformation": {
                "address": {
                    "countryId": "US",
                    "region": "Florida",
                    "regionId": "18",
                    "postcode": null,
                    "city": "",
                    "extension_attributes": {
                        "advanced_conditions": {
                            "payment_method": null,
                            "city": "",
                            "shipping_address_line": [],
                            "billing_address_country": null,
                            "currency": "USD"
                        }
                    }
                }, "shipping_method_code": "freeshipping", "shipping_carrier_code": "freeshipping"
            }
        });

    I.assertEqual(resp.status, 404);
});
