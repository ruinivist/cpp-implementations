# System Design Problem: Stock Availability Notification Service

## Problem Statement

Design a Low-Level Design (LLD) for a "Notify Me When Available" feature on an e-commerce platform. When a high-demand product (like a PS5 or iPhone) is out of stock, customers can subscribe to receive an alert when it becomes available again.

## 1. Functional Requirements (FR)

-   **Product Availability Monitoring:** The system must track the stock quantity of products.
-   **User Subscription:**
    -   Users can subscribe to a specific product to receive availability alerts.
    -   Users can choose their preferred communication channel (Email, SMS, or Push Notification) at the time of subscription.
    -   Users should be able to unsubscribe if they lose interest.
-   **Notification Trigger:**
    -   When a product's stock status changes from "Out of Stock" (0) to "In Stock" (>0), the system must trigger alerts to all subscribed users.
-   **Extensibility:**
    -   The system must support adding new notification channels (e.g., WhatsApp, Discord webhook) in the future with minimal code changes to the core product logic.

## 2. Non-Functional Requirements (NFR)

-   **Decoupling:** The inventory management system (Product logic) should not be tightly coupled with the notification logic (3rd party Email/SMS services).
-   **Real-time/Near Real-time:** Notifications should be sent reasonably quickly after stock is updated.
-   **Scalability:** A single popular product might have 100,000+ subscribers. The notification process should not block the inventory update transaction (e.g., the warehouse manager updating stock shouldn't wait for 100k emails to send before seeing a "Success" message).

---

# Scratch

-   clearly observer pattern ( satisifies decoupling requirement )
-   nfr 2 and 3 need async processing so we can use message queues or threads but let me implement a basic version first

```
interface Subscribable  (observer)
    + subscribe(Subscriber)
    + unsubscribe(Subscriber)
    + notifySubscribers()

interface Subscriber
    + onNotify(Product)

Product (subject) implements Subscribable
    - List<Subscriber> subscribers
    + subscribe(Subscriber)
    + unsubscribe(Subscriber)
    - notifySubscribers()
    + setStock(int)
        - if stock changes from 0 to >0
            - notifySubscribers()
```

# Approach

-   made observer pattern with string messages and global notif ( no filer for specific ids )
-   changed to Event class with runtime polymorphism using base pointer trick
-   used generics to do compile type polymorphism
-   make product id a generic too and change to use map of id to list of subscribers
-   move off the actual notifictions to a thread pool to handle async processing
