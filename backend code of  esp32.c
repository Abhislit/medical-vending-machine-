<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MediVend - Medicine Vending Machine</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
        }

        .header {
            text-align: center;
            color: white;
            margin-bottom: 30px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            backdrop-filter: blur(10px);
        }

        .header h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }

        .header p {
            font-size: 1.1rem;
            opacity: 0.9;
        }

        .main-content {
            display: grid;
            grid-template-columns: 2fr 1fr;
            gap: 30px;
            flex: 1;
        }

        .products-section {
            background: white;
            padding: 25px;
            border-radius: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
        }

        .search-bar {
            width: 100%;
            padding: 15px;
            border: 2px solid #e0e0e0;
            border-radius: 12px;
            font-size: 16px;
            margin-bottom: 25px;
            transition: border-color 0.3s;
        }

        .search-bar:focus {
            outline: none;
            border-color: #667eea;
        }

        .categories {
            display: flex;
            gap: 10px;
            margin-bottom: 25px;
            flex-wrap: wrap;
        }

        .category-btn {
            padding: 10px 20px;
            border: none;
            background: #f0f0f0;
            border-radius: 25px;
            cursor: pointer;
            transition: all 0.3s;
            font-weight: 500;
        }

        .category-btn.active,
        .category-btn:hover {
            background: #667eea;
            color: white;
            transform: translateY(-2px);
        }

        .products-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
            gap: 20px;
        }

        .product-card {
            border: 2px solid #e0e0e0;
            border-radius: 15px;
            padding: 20px;
            transition: all 0.3s;
            background: #fafafa;
        }

        .product-card:hover {
            border-color: #667eea;
            transform: translateY(-5px);
            box-shadow: 0 10px 25px rgba(0,0,0,0.1);
        }

        .product-image {
            width: 100%;
            height: 120px;
            background: linear-gradient(45deg, #e3f2fd, #bbdefb);
            border-radius: 10px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 3rem;
            margin-bottom: 15px;
        }

        .product-name {
            font-size: 1.2rem;
            font-weight: bold;
            margin-bottom: 8px;
            color: #333;
        }

        .product-description {
            color: #666;
            margin-bottom: 10px;
            font-size: 0.9rem;
            line-height: 1.4;
        }

        .product-price {
            font-size: 1.3rem;
            font-weight: bold;
            color: #667eea;
            margin-bottom: 15px;
        }

        .product-stock {
            font-size: 0.9rem;
            color: #28a745;
            margin-bottom: 15px;
        }

        .quantity-controls {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 15px;
        }

        .quantity-btn {
            width: 35px;
            height: 35px;
            border: none;
            background: #667eea;
            color: white;
            border-radius: 50%;
            cursor: pointer;
            font-size: 18px;
            display: flex;
            align-items: center;
            justify-content: center;
            transition: background 0.3s;
        }

        .quantity-btn:hover {
            background: #5a67d8;
        }

        .quantity-display {
            font-size: 1.2rem;
            font-weight: bold;
            min-width: 40px;
            text-align: center;
        }

        .add-to-cart {
            width: 100%;
            padding: 12px;
            background: #28a745;
            color: white;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            font-size: 16px;
            font-weight: 600;
            transition: all 0.3s;
        }

        .add-to-cart:hover {
            background: #218838;
            transform: translateY(-1px);
        }

        .add-to-cart:disabled {
            background: #ccc;
            cursor: not-allowed;
            transform: none;
        }

        .cart-section {
            background: white;
            padding: 25px;
            border-radius: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
            height: fit-content;
            sticky: top-20px;
        }

        .cart-header {
            font-size: 1.5rem;
            font-weight: bold;
            margin-bottom: 20px;
            color: #333;
            text-align: center;
        }

        .cart-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 15px 0;
            border-bottom: 1px solid #e0e0e0;
        }

        .cart-item-info {
            flex: 1;
        }

        .cart-item-name {
            font-weight: bold;
            margin-bottom: 5px;
        }

        .cart-item-price {
            color: #666;
            font-size: 0.9rem;
        }

        .cart-item-quantity {
            font-weight: bold;
            color: #667eea;
            margin-left: 10px;
        }

        .remove-item {
            background: #dc3545;
            color: white;
            border: none;
            padding: 5px 10px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 12px;
        }

        .remove-item:hover {
            background: #c82333;
        }

        .cart-total {
            padding: 20px 0;
            border-top: 2px solid #e0e0e0;
            margin-top: 15px;
        }

        .total-amount {
            font-size: 1.5rem;
            font-weight: bold;
            color: #333;
            text-align: center;
        }

        .checkout-btn {
            width: 100%;
            padding: 15px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            border: none;
            border-radius: 12px;
            font-size: 18px;
            font-weight: bold;
            cursor: pointer;
            margin-top: 20px;
            transition: all 0.3s;
        }

        .checkout-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 8px 20px rgba(102, 126, 234, 0.4);
        }

        .checkout-btn:disabled {
            background: #ccc;
            cursor: not-allowed;
            transform: none;
            box-shadow: none;
        }

        .payment-modal {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0,0,0,0.8);
            z-index: 1000;
            align-items: center;
            justify-content: center;
        }

        .payment-content {
            background: white;
            padding: 40px;
            border-radius: 20px;
            max-width: 500px;
            width: 90%;
            text-align: center;
        }

        .payment-form {
            margin-top: 20px;
        }

        .form-group {
            margin-bottom: 20px;
            text-align: left;
        }

        .form-group label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
        }

        .form-group input {
            width: 100%;
            padding: 12px;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            font-size: 16px;
        }

        .form-group input:focus {
            outline: none;
            border-color: #667eea;
        }

        .payment-buttons {
            display: flex;
            gap: 15px;
            margin-top: 25px;
        }

        .payment-buttons button {
            flex: 1;
            padding: 12px;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s;
        }

        .pay-btn {
            background: #28a745;
            color: white;
        }

        .pay-btn:hover {
            background: #218838;
        }

        .cancel-btn {
            background: #6c757d;
            color: white;
        }

        .cancel-btn:hover {
            background: #5a6268;
        }

        .success-message {
            display: none;
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: #28a745;
            color: white;
            padding: 30px 40px;
            border-radius: 15px;
            text-align: center;
            z-index: 1001;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
        }

        .empty-cart {
            text-align: center;
            color: #666;
            padding: 40px 20px;
        }

        .empty-cart-icon {
            font-size: 4rem;
            margin-bottom: 15px;
            opacity: 0.5;
        }

        @media (max-width: 768px) {
            .main-content {
                grid-template-columns: 1fr;
            }
            
            .products-grid {
                grid-template-columns: 1fr;
            }
            
            .header h1 {
                font-size: 2rem;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header class="header">
            <h1>🏥 MediVend</h1>
            <p>Your 24/7 Medicine Vending Solution</p>
        </header>

        <main class="main-content">
            <section class="products-section">
                <input type="text" class="search-bar" placeholder="Search for medicines..." id="searchBar">
                
                <div class="categories">
                    <button class="category-btn active" data-category="all">All</button>
                    <button class="category-btn" data-category="pain-relief">Pain Relief</button>
                    <button class="category-btn" data-category="cold-flu">Cold & Flu</button>
                    <button class="category-btn" data-category="digestive">Digestive</button>
                    <button class="category-btn" data-category="vitamins">Vitamins</button>
                    <button class="category-btn" data-category="first-aid">First Aid</button>
                </div>

                <div class="products-grid" id="productsGrid">
                    <!-- Products will be populated by JavaScript -->
                </div>
            </section>

            <section class="cart-section">
                <div class="cart-header">🛒 Your Cart</div>
                <div id="cartItems">
                    <div class="empty-cart">
                        <div class="empty-cart-icon">🛒</div>
                        <p>Your cart is empty<br>Add some medicines to get started!</p>
                    </div>
                </div>
                <div class="cart-total" id="cartTotal" style="display: none;">
                    <div class="total-amount">Total: ₹<span id="totalAmount">0</span></div>
                </div>
                <button class="checkout-btn" id="checkoutBtn" disabled>Proceed to Payment</button>
            </section>
        </main>
    </div>

    <!-- Payment Modal -->
    <div class="payment-modal" id="paymentModal">
        <div class="payment-content">
            <h2>💳 Payment Details</h2>
            <p>Total Amount: ₹<span id="paymentTotal">0</span></p>
            
            <form class="payment-form" id="paymentForm">
                <div class="form-group">
                    <label for="cardNumber">Card Number</label>
                    <input type="text" id="cardNumber" placeholder="1234 5678 9012 3456" required>
                </div>
                
                <div class="form-group">
                    <label for="cardName">Cardholder Name</label>
                    <input type="text" id="cardName" placeholder="John Doe" required>
                </div>
                
                <div style="display: flex; gap: 15px;">
                    <div class="form-group" style="flex: 1;">
                        <label for="expiryDate">Expiry Date</label>
                        <input type="text" id="expiryDate" placeholder="MM/YY" required>
                    </div>
                    
                    <div class="form-group" style="flex: 1;">
                        <label for="cvv">CVV</label>
                        <input type="text" id="cvv" placeholder="123" required>
                    </div>
                </div>
                
                <div class="payment-buttons">
                    <button type="button" class="cancel-btn" id="cancelPayment">Cancel</button>
                    <button type="submit" class="pay-btn">Pay Now</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Success Message -->
    <div class="success-message" id="successMessage">
        <h2>✅ Payment Successful!</h2>
        <p>Your medicines are being dispensed...<br>Please collect them from the vending machine.</p>
        <p><strong>Transaction ID: <span id="transactionId"></span></strong></p>
    </div>

    <script>
        // Medicine inventory data
        const medicines = [
            {
                id: 1,
                name: "Paracetamol 500mg",
                description: "Pain relief and fever reducer",
                price: 25,
                category: "pain-relief",
                stock: 15,
                emoji: "💊"
            },
            {
                id: 2,
                name: "Ibuprofen 400mg",
                description: "Anti-inflammatory pain reliever",
                price: 35,
                category: "pain-relief",
                stock: 12,
                emoji: "💊"
            },
            {
                id: 3,
                name: "Cough Syrup",
                description: "Honey-based cough relief",
                price: 85,
                category: "cold-flu",
                stock: 8,
                emoji: "🧴"
            },
            {
                id: 4,
                name: "Vitamin C Tablets",
                description: "Immune system booster (30 tablets)",
                price: 65,
                category: "vitamins",
                stock: 20,
                emoji: "🍊"
            },
            {
                id: 5,
                name: "Antacid Tablets",
                description: "Quick relief from acidity",
                price: 45,
                category: "digestive",
                stock: 18,
                emoji: "💊"
            },
            {
                id: 6,
                name: "Band-Aid Pack",
                description: "Adhesive bandages (10 pieces)",
                price: 55,
                category: "first-aid",
                stock: 25,
                emoji: "🩹"
            },
            {
                id: 7,
                name: "Throat Lozenges",
                description: "Soothing throat relief",
                price: 30,
                category: "cold-flu",
                stock: 16,
                emoji: "🍬"
            },
            {
                id: 8,
                name: "Multivitamin",
                description: "Complete nutrition supplement",
                price: 120,
                category: "vitamins",
                stock: 14,
                emoji: "💊"
            },
            {
                id: 9,
                name: "Antiseptic Solution",
                description: "Wound cleaning solution",
                price: 40,
                category: "first-aid",
                stock: 10,
                emoji: "🧴"
            },
            {
                id: 10,
                name: "Digestive Enzymes",
                description: "Helps with digestion",
                price: 95,
                category: "digestive",
                stock: 7,
                emoji: "💊"
            }
        ];

        let cart = [];
        let quantities = {};

        // Initialize quantities
        medicines.forEach(medicine => {
            quantities[medicine.id] = 0;
        });

        // DOM elements
        const productsGrid = document.getElementById('productsGrid');
        const cartItems = document.getElementById('cartItems');
        const totalAmount = document.getElementById('totalAmount');
        const cartTotal = document.getElementById('cartTotal');
        const checkoutBtn = document.getElementById('checkoutBtn');
        const paymentModal = document.getElementById('paymentModal');
        const successMessage = document.getElementById('successMessage');
        const searchBar = document.getElementById('searchBar');

        // Render products
        function renderProducts(productsToShow = medicines) {
            productsGrid.innerHTML = '';
            
            productsToShow.forEach(medicine => {
                const productCard = document.createElement('div');
                productCard.className = 'product-card';
                productCard.innerHTML = `
                    <div class="product-image">${medicine.emoji}</div>
                    <div class="product-name">${medicine.name}</div>
                    <div class="product-description">${medicine.description}</div>
                    <div class="product-price">₹${medicine.price}</div>
                    <div class="product-stock">Stock: ${medicine.stock} units</div>
                    <div class="quantity-controls">
                        <button class="quantity-btn" onclick="changeQuantity(${medicine.id}, -1)">-</button>
                        <span class="quantity-display" id="qty-${medicine.id}">${quantities[medicine.id]}</span>
                        <button class="quantity-btn" onclick="changeQuantity(${medicine.id}, 1)">+</button>
                    </div>
                    <button class="add-to-cart" onclick="addToCart(${medicine.id})" 
                            ${quantities[medicine.id] === 0 ? 'disabled' : ''}>
                        Add to Cart
                    </button>
                `;
                productsGrid.appendChild(productCard);
            });
        }

        // Change quantity
        function changeQuantity(medicineId, change) {
            const medicine = medicines.find(m => m.id === medicineId);
            const newQuantity = quantities[medicineId] + change;
            
            if (newQuantity >= 0 && newQuantity <= medicine.stock) {
                quantities[medicineId] = newQuantity;
                document.getElementById(`qty-${medicineId}`).textContent = newQuantity;
                
                // Update add to cart button
                const addToCartBtn = document.querySelector(`button[onclick="addToCart(${medicineId})"]`);
                addToCartBtn.disabled = newQuantity === 0;
            }
        }

        // Add to cart
        function addToCart(medicineId) {
            const medicine = medicines.find(m => m.id === medicineId);
            const quantity = quantities[medicineId];
            
            if (quantity > 0) {
                const existingItem = cart.find(item => item.id === medicineId);
                
                if (existingItem) {
                    existingItem.quantity += quantity;
                } else {
                    cart.push({
                        ...medicine,
                        quantity: quantity
                    });
                }
                
                // Update stock
                medicine.stock -= quantity;
                
                // Reset quantity
                quantities[medicineId] = 0;
                
                // Re-render
                renderProducts();
                renderCart();
            }
        }

        // Remove from cart
        function removeFromCart(medicineId) {
            const cartItem = cart.find(item => item.id === medicineId);
            if (cartItem) {
                // Return stock
                const medicine = medicines.find(m => m.id === medicineId);
                medicine.stock += cartItem.quantity;
                
                // Remove from cart
                cart = cart.filter(item => item.id !== medicineId);
                
                // Re-render
                renderProducts();
                renderCart();
            }
        }

        // Render cart
        function renderCart() {
            if (cart.length === 0) {
                cartItems.innerHTML = `
                    <div class="empty-cart">
                        <div class="empty-cart-icon">🛒</div>
                        <p>Your cart is empty<br>Add some medicines to get started!</p>
                    </div>
                `;
                cartTotal.style.display = 'none';
                checkoutBtn.disabled = true;
                return;
            }

            let cartHTML = '';
            let total = 0;

            cart.forEach(item => {
                const itemTotal = item.price * item.quantity;
                total += itemTotal;
                
                cartHTML += `
                    <div class="cart-item">
                        <div class="cart-item-info">
                            <div class="cart-item-name">${item.name}</div>
                            <div class="cart-item-price">₹${item.price} × ${item.quantity} = ₹${itemTotal}</div>
                        </div>
                        <button class="remove-item" onclick="removeFromCart(${item.id})">Remove</button>
                    </div>
                `;
            });

            cartItems.innerHTML = cartHTML;
            totalAmount.textContent = total;
            cartTotal.style.display = 'block';
            checkoutBtn.disabled = false;
        }

        // Category filtering
        document.addEventListener('DOMContentLoaded', function() {
            const categoryBtns = document.querySelectorAll('.category-btn');
            
            categoryBtns.forEach(btn => {
                btn.addEventListener('click', function() {
                    // Remove active class from all buttons
                    categoryBtns.forEach(b => b.classList.remove('active'));
                    // Add active class to clicked button
                    this.classList.add('active');
                    
                    const category = this.dataset.category;
                    let filteredMedicines = medicines;
                    
                    if (category !== 'all') {
                        filteredMedicines = medicines.filter(m => m.category === category);
                    }
                    
                    renderProducts(filteredMedicines);
                });
            });
        });

        // Search functionality
        searchBar.addEventListener('input', function() {
            const searchTerm = this.value.toLowerCase();
            const filteredMedicines = medicines.filter(medicine =>
                medicine.name.toLowerCase().includes(searchTerm) ||
                medicine.description.toLowerCase().includes(searchTerm)
            );
            renderProducts(filteredMedicines);
        });

        // Checkout process
        checkoutBtn.addEventListener('click', function() {
            const total = cart.reduce((sum, item) => sum + (item.price * item.quantity), 0);
            document.getElementById('paymentTotal').textContent = total;
            paymentModal.style.display = 'flex';
        });

        // Cancel payment
        document.getElementById('cancelPayment').addEventListener('click', function() {
            paymentModal.style.display = 'none';
        });

        // Process payment
        document.getElementById('paymentForm').addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Simulate payment processing
            paymentModal.style.display = 'none';
            
            // Generate transaction ID
            const transactionId = 'TXN' + Date.now().toString().slice(-8);
            document.getElementById('transactionId').textContent = transactionId;
            
            // Show success message
            successMessage.style.display = 'block';
            
            // Simulate vending machine dispensing
            setTimeout(() => {
                // Clear cart
                cart = [];
                medicines.forEach(medicine => {
                    quantities[medicine.id] = 0;
                });
                
                // Hide success message and refresh
                successMessage.style.display = 'none';
                renderProducts();
                renderCart();
                
                // In real implementation, send dispensing command to vending machine
                console.log('Dispensing medicines...');
            }, 4000);
        });

        // Format card number input
        document.getElementById('cardNumber').addEventListener('input', function() {
            let value = this.value.replace(/\s/g, '').replace(/\D/g, '');
            let formattedValue = value.replace(/(\d{4})(?=\d)/g, '$1 ');
            if (formattedValue.length <= 19) {
                this.value = formattedValue;
            }
        });

        // Format expiry date input
        document.getElementById('expiryDate').addEventListener('input', function() {
            let value = this.value.replace(/\D/g, '');
            if (value.length >= 2) {
                value = value.substring(0, 2) + '/' + value.substring(2, 4);
            }
            this.value = value;
        });

        // Format CVV input
        document.getElementById('cvv').addEventListener('input', function() {
            this.value = this.value.replace(/\D/g, '').substring(0, 3);
        });

        // Initial render
        renderProducts();
        renderCart();
        
        // Check ESP32 connection on page load
        checkESP32Connection();
        
        // Sync inventory on page load
        syncInventory();
        
        // Periodically check connection and sync inventory
        setInterval(() => {
            checkESP32Connection();
        }, 30000); // Check every 30 seconds
        
        setInterval(() => {
            if (!cart.length) { // Only sync when cart is empty to avoid conflicts
                syncInventory();
            }
        }, 60000); // Sync every minute
    </script>
</body>
</html>
