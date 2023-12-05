#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <windows.h>
#include <conio.h>

COORD coord = {0, 0};

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}
struct Product
{
    char ProductName[50];
    float price;
    int quantity;
};

typedef struct Product Product;

struct Category
{
    int id;
    int ProductCount;
    char CategoryName[50];
    Product *products;
};

typedef struct Category Category;

Category *categories = NULL;
int categoryCount = 0;

//ADDING CATEGORY

void AddCategory(Category **allCategory)
{
    int EnteringMore = 1;

    do
    {
        int Id;
        int Valid_ID = 1;

        do
        {
            Valid_ID = 1;

            printf("Enter Category ID: ");
            scanf("%d", &Id);

            for (int i = 0; i < categoryCount; ++i)
            {
                if ((*allCategory)[i].id == Id)
                {
                    printf("ID already exists. Please enter a different ID\n");
                    Valid_ID = 0;
                }
            }
        }
        while (!Valid_ID);

        Category new_category;

        categoryCount++;
        *allCategory = realloc(*allCategory, categoryCount * sizeof(Category));

        new_category.id = Id;
        new_category.products = NULL;
        new_category.ProductCount = 0;

        int isDuplicate = 0;
        do
        {
            isDuplicate = 0;
            printf("Enter Category Name: ");
            scanf("%s", new_category.CategoryName);

            for (int i = 0; i < categoryCount - 1; ++i)
            {
                if (strcmp((*allCategory)[i].CategoryName, new_category.CategoryName) == 0)
                {
                    printf("Category Name already exists. Please enter a different name.\n");
                    isDuplicate = 1;
                }
            }

        }
        while (isDuplicate);

        (*allCategory)[categoryCount - 1] = new_category;

        printf("Do you want to add more categories? (Y/N): ");
        char response;
        scanf(" %c", &response);

        if (response != 'y' && response != 'Y')
        {
            EnteringMore = 0;
        }
    }
    while (EnteringMore);

    printf("Category added successfully! Press any key to return to the menu...");
    getch();
    menu();
}

//ADDING PRODUCT

void AddProductToCategory(Category **allCategory)
{
    int EnteringMore = 1;
    if (categoryCount == 0)
    {
        printf("There are no categories available. Please add a category first.\n");
        printf("Press any key to return to the menu...");
        getch();
        menu();
    }
    else
    {
        DisplayCategories(*allCategory, 0);
        int categoryID, FindID = -1;
        printf("Enter the category ID to add the product: ");
        scanf("%d", &categoryID);

        do
        {
            FindID = -1;
            for (int i = 0; i < categoryCount; ++i)
            {
                if ((*allCategory)[i].id == categoryID)
                {
                    FindID = i;
                }
            }

            if (FindID == -1)
            {
                printf("Category with ID %d does not exist.\n", categoryID);
                printf("Do you want to enter another category ID? (Y/N): ");
                char response;
                scanf(" %c", &response);
                if (response != 'y' && response != 'Y')
                {
                    printf("Press any key to return to the menu...");
                    getch();
                    menu();
                }
                else
                {
                    printf("Enter the new category ID to add the product: ");
                    scanf("%d", &categoryID);
                }
            }
        }
        while (FindID == -1);

        do
        {
            // Increase product count for the selected category
            (*allCategory)[FindID].ProductCount++;

            (*allCategory)[FindID].products = realloc((*allCategory)[FindID].products, (*allCategory)[FindID].ProductCount * sizeof(Product));

            // Input name for the new product
            printf("Enter Product Name: ");
            scanf("%s", (*allCategory)[FindID].products[(*allCategory)[FindID].ProductCount - 1].ProductName);
            int ExistProduct = -1;

            // Check if there is a duplicate name
            for (int i = 0; i < (*allCategory)[FindID].ProductCount - 1; ++i)
            {
                if (strcmp((*allCategory)[FindID].products[i].ProductName, (*allCategory)[FindID].products[(*allCategory)[FindID].ProductCount - 1].ProductName) == 0)
                {
                    ExistProduct = i;
                }
            }

            if (ExistProduct != -1)
            {
                printf("Product with the same name already exists. Please enter a different name.\n");
                (*allCategory)[FindID].ProductCount--;

            }
            else
            {
                // Add the new product here
                printf("Enter Product Price: ");
                scanf("%f", &(*allCategory)[FindID].products[(*allCategory)[FindID].ProductCount - 1].price);

                printf("Enter Product Quantity: ");
                scanf("%d", &(*allCategory)[FindID].products[(*allCategory)[FindID].ProductCount - 1].quantity);

                printf("Product added to category with ID: %d\n", FindID + 1);
            }

            printf("Do you want to add more products to this category (Y/N) or change category (C)?: ");
            char response3;
            scanf(" %c", &response3);

            if (response3 == 'n' || response3 == 'N')
            {
                EnteringMore = 0;
            }
            else if (response3 == 'c' || response3 == 'C')
            {
                int newCategoryID;
                printf("Enter the new category ID to add the product: ");
                scanf("%d", &newCategoryID);
                int NewFindID = -1;
                for (int i = 0; i < categoryCount; ++i)
                {
                    if ((*allCategory)[i].id == newCategoryID)
                    {
                        NewFindID = i;
                    }
                }

                if (NewFindID == -1)
                {
                    printf("Category with ID %d does not exist.\n", newCategoryID);
                    printf("Press any key to return to the menu...");
                    getch();
                    menu();
                }
                else
                {
                    FindID = NewFindID;
                }
            }
        }
        while (EnteringMore);

    }
    printf("Press any key to return to the menu...");
    getch();
    menu();
}


void EditProduct(Category **allCategory)
{
    int categoryID, productIndex;
    char productName[50];
    int choice;

    if (categoryCount == 0)
    {
        printf("There are no categories available. Please add a category first.\n");
        printf("Press any key to return to the menu...");
        getch();
        menu();
    }
    else
    {
        DisplayCategories(*allCategory, 0);
        printf("Enter the category ID to edit a product: ");
        scanf("%d", &categoryID);

        int findID = -1;
        for (int i = 0; i < categoryCount; ++i)
        {
            if ((*allCategory)[i].id == categoryID)
            {
                findID = i;
            }
        }

        if (findID == -1 || (*allCategory)[findID].ProductCount == 0)
        {
            printf("Category with ID %d does not exist or it has no products.\n", categoryID);
            printf("Press any key to return to the menu...");
            getch();
            menu();
        }
        else
        {
            printf("Enter the name of the product to edit: ");
            scanf("%s", productName);

            productIndex = -1;
            for (int i = 0; i < (*allCategory)[findID].ProductCount; ++i)
            {
                if (strcmp(productName, (*allCategory)[findID].products[i].ProductName) == 0)
                {
                    productIndex = i;
                }
            }

            if (productIndex == -1)
            {
                printf("Product with name '%s' does not exist in category ID %d.\n", productName, categoryID);
                printf("Press any key to return to the menu...");
                getch();
                menu();
            }
            else
            {
                printf("What do you want to edit?\n");
                printf("1. Name\n2. Quantity\n3. Price\nEnter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    printf("Enter new name: ");
                    scanf("%s", (*allCategory)[findID].products[productIndex].ProductName);
                    break;
                case 2:
                    printf("Enter new quantity: ");
                    scanf("%d", &(*allCategory)[findID].products[productIndex].quantity);
                    break;
                case 3:
                    printf("Enter new price: ");
                    scanf("%f", &(*allCategory)[findID].products[productIndex].price);
                    break;
                default:
                    printf("Invalid choice!\n");
                    break;
                }

                printf("Product details updated successfully!\n");
                printf("Press any key to return to the menu...");
                getch();
                menu();
            }
        }
    }
}



//SELLING
void SellProduct(Category **allCategory)
{
    char ProductName[50];
    int QuantityToSell;

    if (categoryCount == 0)
    {
        printf("There are no categories available. Please add a category first.\n");
        printf("Press any key to return to the menu...");
        getch();

        menu();
    }
    else
    {
        DisplayCategories(*allCategory,0);
        int categoryID, FindID = -1;
        printf("Enter the category ID to sell a product from: ");
        scanf("%d", &categoryID);


        do
        {
            FindID = -1;
            for (int i = 0; i < categoryCount; ++i)
            {
                if ((*allCategory)[i].id == categoryID)
                {
                    FindID = i;
                }
            }

            if (FindID == -1)
            {
                printf("Category with ID %d does not exist.\n", categoryID);
                printf("Do you want to enter another category ID? (Y/N): ");
                char response;
                scanf(" %c", &response);
                if (response != 'y' && response != 'Y')
                {
                    printf("Press any key to return to the menu...");
                    getch();
                    menu();
                }
                else
                {
                    printf("Enter the category ID to sell a product from: ");
                    scanf("%d", &categoryID);
                }
            }
        }
        while (FindID == -1);   // Repeat until a valid category ID is entered

        if ((*allCategory)[FindID].ProductCount == 0)
        {
            printf("No products available in this category to sell.\n");
            printf("Do you want to enter another category ID? (Y/N): ");
            char response;
            scanf(" %c", &response);
            if (response != 'y' && response != 'Y')
            {
                printf("Press any key to return to the menu...");
                getch();
                menu();
            }
            else
            {
                SellProduct(allCategory); // Recursive call to SellProduct to prompt for category ID again
            }
        }
        else
        {
            printf("Enter Product Name : ");
            scanf("%s",&ProductName);
            int ProductIndex=-1;
            for (int i = 0; i < (*allCategory)[FindID].ProductCount; ++i)
            {
                if (strcmp(ProductName, (*allCategory)[FindID].products[i].ProductName) == 0)
                {
                    ProductIndex = i;
                }
            }
            if(ProductIndex==-1)
            {
                printf("product with Name %s does not exist.\n", ProductName);
                printf("Press any key to return to the menu...");
                getch();
                menu();
            }
            else
            {

                printf("Enter the quantity to sell: ");
                scanf("%d",&QuantityToSell);
                if (QuantityToSell <= 0 || QuantityToSell > (*allCategory)[FindID].products[ProductIndex].quantity)
                {
                    printf("Invalid quantity entered or insufficient quantity available.\n");
                    printf("Press any key to return to the menu...");
                    getch();
                    menu();
                }
                else
                {
                    (*allCategory)[FindID].products[ProductIndex].quantity -= QuantityToSell;
                    printf("Successfully sold %d quantity of %s.\n", QuantityToSell, ProductName);
                    printf("Press any key to return to the menu...");
                    getch();
                    menu();
                }
            }

        }
    }

}





//MOVING
void MoveProduct(Category **allCategory)
{
    int ID_SenderCategory, ID_ReceiverCategory;
    int FindID_Sender = -1, FindID_Receiver = -1;
    char productName[50];
    int ProductIndex = -1;
    int EnteringMore = 1;
    char choice;

    if (categoryCount == 0)
    {
        printf("There are no categories available. Please add a category first.\n");
        printf("Press any key to return to the menu...");
        getch();
        return;
    }
    else if (categoryCount < 2)
    {
        printf("There should be at least two categories to move products.\n");
        printf("Press any key to return to the menu...");
        getch();
        return;
    }
    else
    {
        DisplayCategories(*allCategory, 0);
        do
        {
            printf("Enter the category ID you will move from: ");
            scanf("%d", &ID_SenderCategory);

            do
            {
                FindID_Sender = -1;

                for (int i = 0; i < categoryCount; ++i)
                {
                    if ((*allCategory)[i].id == ID_SenderCategory)
                    {
                        FindID_Sender = i;
                    }
                }
                if (FindID_Sender == -1 || (*allCategory)[ID_SenderCategory - 1].ProductCount == 0)
                {
                    printf("Category with ID %d does not exist or has no products to move.\n", ID_SenderCategory);
                    printf("Do you want to enter another ID? (Y/N): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y')
                    {
                        printf("Press any key to return to the menu...");
                        getch();
                        return;
                    }
                    else
                    {
                        printf("Enter the correct category ID you will move from: ");
                        scanf("%d", &ID_SenderCategory);
                    }
                }
            }
            while (FindID_Sender == -1 || (*allCategory)[ID_SenderCategory - 1].ProductCount == 0);

            printf("Enter Name of product you want to move : ");
            scanf(" %s", &productName);
            do
            {
                // Check if the product exists in the sender category
                for (int i = 0; i < (*allCategory)[ID_SenderCategory-1].ProductCount; ++i)
                {
                    if (strcmp((*allCategory)[ID_SenderCategory-1].products[i].ProductName, productName) == 0)
                    {
                        ProductIndex = i;

                    }
                }

                if (ProductIndex == -1)
                {
                    printf("Product with name '%s' does not exist in the source category.\n", productName);
                    printf("Do you want to enter another Name? (Y/N): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y')
                    {
                        printf("Press any key to return to the menu...");
                        getch();
                        return;
                    }
                    else
                    {
                        printf("Enter right Name of product you want to move : ");
                        scanf(" %s", &productName);
                    }
                }
            }
            while(ProductIndex==-1);

            printf("Enter the category ID you will move to: ");
            scanf("%d", &ID_ReceiverCategory);

            do
            {
                for (int i = 0; i < categoryCount; ++i)
                {
                    if ((*allCategory)[i].id == ID_ReceiverCategory)
                    {
                        FindID_Receiver = i;
                    }
                }
                if (FindID_Receiver == -1)
                {
                    printf("Category with ID %d does not exist.\n", ID_ReceiverCategory);
                    printf("Do you want to enter another ID? (Y/N): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y')
                    {
                        printf("Press any key to return to the menu...");
                        getch();
                        return;
                    }
                    else
                    {
                        printf("Enter the correct category ID you will move to: ");
                        scanf("%d", &ID_ReceiverCategory);
                    }
                }
            }
            while (FindID_Receiver == -1);

            int productFound = 0;
            for (int i = 0; i < (*allCategory)[ID_ReceiverCategory - 1].ProductCount; ++i)
            {
                if (strcmp((*allCategory)[ID_ReceiverCategory - 1].products[i].ProductName, productName) == 0)
                {
                    printf("Product '%s' already exists in this category.\n", productName);
                    (*allCategory)[ID_ReceiverCategory - 1].products[i].quantity += (*allCategory)[ID_SenderCategory - 1].products[ProductIndex].quantity;

                    for (int j = ProductIndex; j < (*allCategory)[ID_SenderCategory - 1].ProductCount - 1; ++j)
                    {
                        strcpy((*allCategory)[ID_SenderCategory - 1].products[j].ProductName, (*allCategory)[ID_SenderCategory - 1].products[j + 1].ProductName);
                        (*allCategory)[ID_SenderCategory - 1].products[j].price = (*allCategory)[ID_SenderCategory - 1].products[j + 1].price;
                        (*allCategory)[ID_SenderCategory - 1].products[j].quantity = (*allCategory)[ID_SenderCategory - 1].products[j + 1].quantity;
                    }

                    (*allCategory)[ID_SenderCategory - 1].ProductCount--;

                    printf("Product '%s' moved to destination category.\n", productName);
                    productFound = 1;
                }
            }

            if (!productFound)
            {
                (*allCategory)[ID_ReceiverCategory - 1].products = realloc((*allCategory)[ID_ReceiverCategory - 1].products, ((*allCategory)[ID_ReceiverCategory - 1].ProductCount + 1) * sizeof(Product));

                strcpy((*allCategory)[ID_ReceiverCategory - 1].products[(*allCategory)[ID_ReceiverCategory - 1].ProductCount].ProductName, productName);
                (*allCategory)[ID_ReceiverCategory - 1].products[(*allCategory)[ID_ReceiverCategory - 1].ProductCount].price = (*allCategory)[ID_SenderCategory - 1].products[ProductIndex].price;
                (*allCategory)[ID_ReceiverCategory - 1].products[(*allCategory)[ID_ReceiverCategory - 1].ProductCount].quantity = (*allCategory)[ID_SenderCategory - 1].products[ProductIndex].quantity;
                (*allCategory)[ID_ReceiverCategory - 1].ProductCount++;

                for (int i = ProductIndex; i < (*allCategory)[ID_SenderCategory - 1].ProductCount - 1; ++i)
                {
                    strcpy((*allCategory)[ID_SenderCategory - 1].products[i].ProductName, (*allCategory)[ID_SenderCategory - 1].products[i + 1].ProductName);
                    (*allCategory)[ID_SenderCategory - 1].products[i].price = (*allCategory)[ID_SenderCategory - 1].products[i + 1].price;
                    (*allCategory)[ID_SenderCategory - 1].products[i].quantity = (*allCategory)[ID_SenderCategory - 1].products[i + 1].quantity;
                }

                (*allCategory)[ID_SenderCategory - 1].ProductCount--;
            }
            printf("Product '%s' moved successfully from category %d to category %d.\n", productName, ID_SenderCategory, ID_ReceiverCategory);

            printf("Do you want to make another Move? (Y/N): ");
            char response;
            scanf(" %c", &response);

            if (response != 'y' && response != 'Y')
            {
                EnteringMore = 0;
            }
        }
        while (EnteringMore);
        printf("Press any key to return to the menu...");
        getch();
        menu();
    }
}


//DISPLAYING


void DisplayCategories(Category *allCategory,int ShowMessage)
{
    if (categoryCount == 0)
    {
        printf("There are no categories available. Please add a category first.\n");
        printf("Press any key to return to the menu...");
        getch();
        menu();

    }
    else
    {
        gotoxy(40,0);
        SetColor(3);
        printf("**Available Categories AND Products**\n\n");
        printf("    ***************************************************************************************************************\n");
        SetColor(15);
        for (int i = 0; i < categoryCount; ++i)
        {
            printf("(%d): Category Name : %s\n\n", allCategory[i].id, allCategory[i].CategoryName);

            if (allCategory[i].ProductCount == 0)
            {
                printf("No products available in this Category.\n\n");
            }
            else
            {
                for (int j = 0; j < allCategory[i].ProductCount; ++j)
                {
                    printf("Product (%d): Product Name: %s | Product Price: %.2f | Product Quantity: %d\n\n", j + 1,allCategory[i].products[j].ProductName,allCategory[i].products[j].price,allCategory[i].products[j].quantity);

                }
            }
        }
        SetColor(3);

        printf("    ***************************************************************************************************************\n");
        SetColor(15);

    }
    if (ShowMessage)
    {
        printf("Press any key to return to the menu...");
        getch();
        menu();

    }

}


void menu()
{
    char selection[8][20] = {"Add Category","Add Product","Edit Product", "Sell Product", "Move Product", "Display","Exit"};
    char ch;
    int selectOption = 0;


    while (1)
    {


        system("cls");
        gotoxy(40,5);
        SetColor(3);
        printf("Welcome To Our MARKET\n");
        SetColor(15);
        for (int i = 0; i < 7; ++i)
        {
            gotoxy(40, 7 + i);
            if (i == selectOption)
            {
                SetColor(11);
                printf("-> %s\n", selection[i]);
            }
            else
            {
                SetColor(15);

                printf("   %s\n", selection[i]);
            }
        }

        ch = getch();

        switch (ch)
        {
        case 72:  // UP arrow key
            if (selectOption == 0)
            {
                selectOption = 6;
            }
            else
            {
                selectOption--;
            }
            break;
        case 80:  // DOWN arrow key
            if (selectOption == 6)
            {
                selectOption = 0;
            }
            else
            {
                selectOption++;
            }
            break;
        case 13:  // Enter key
            if (selectOption == 0)
            {
                system("cls");
                AddCategory(&categories);
            }
            // Add conditions for Sell, Move, Display, Search, and Exit here
            else if (selectOption == 1)
            {
                system("cls");
                AddProductToCategory(&categories);
            }
            else if (selectOption == 2)
            {
                system("cls");
                EditProduct(&categories);
            }

            else if (selectOption == 3)
            {
                system("cls");
                SellProduct(&categories);
            }
            else if (selectOption == 4)
            {
                system("cls");
                MoveProduct(&categories);
            }
            else if(selectOption==5)
            {
                system("cls");
                DisplayCategories(categories,1);
            }
            else if(selectOption==6)
            {
                system("cls");
                exit(0);
            }
            break;
        case 27:  // ESC key
            return 0; // Return from menu if ESC is pressed
        }
    }
}

int main()
{

    menu();
    return 0;
}
