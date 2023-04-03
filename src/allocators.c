/*
	printf("Default allocator.\n");
	{
		f64 const start = timeStamp();
	
		isize *ptr = null;
	
		ptr = context.allocator->procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
			.size_to_be_allocated_or_resized = sizeof(isize) * 128,
		});
	
		ptr = context.allocator->procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
			.ptr_to_be_resized_or_freed      = ptr,
			.size_to_be_allocated_or_resized = sizeof(isize) * 192,
		});
	
		context.allocator->procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription){
			.ptr_to_be_resized_or_freed = ptr,
		});
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Temporary-storage-based allocator.\n");
	{
		f64 const start = timeStamp();
	
		isize *ptr = null;
	
		ptr = temporary_allocator.procedure(ALLOCATOR_MODE_ALLOCATE, &(AllocatorDescription){
			.size_to_be_allocated_or_resized = sizeof(isize) * 128,
		});
	
		ptr = temporary_allocator.procedure(ALLOCATOR_MODE_RESIZE, &(AllocatorDescription){
			.ptr_to_be_resized_or_freed      = ptr,
			.size_to_be_allocated_or_resized = sizeof(isize) * 192,
		});
	
		temporary_allocator.procedure(ALLOCATOR_MODE_FREE, &(AllocatorDescription){
			.ptr_to_be_resized_or_freed = ptr,
		});
	
		temporaryStorageReset();
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Default allocator (wrapped).\n");
	{
		f64 const start = timeStamp();
	
		isize *ptr = null;
	
		ptr = alloc(sizeof(isize) * 128);
		ptr = resize(ptr, sizeof(isize) * 192);
		free(ptr);
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Temporary-storage-based allocator (wrapped).\n");
	{
		f64 const start = timeStamp();
	
		contextSetAllocators(&temporary_allocator);
	
		isize *ptr = null;
	
		ptr = alloc(sizeof(isize) * 128);
		ptr = resize(ptr, sizeof(isize) * 192);
		free(ptr);
	
		f64 const end = timeStamp();
	
		{
			Arena arena = arenaCreate(&(ArenaDescription) {
				0,
			});
	
			Allocator arena_allocator = arenaGetAllocator(&arena);
	
			contextSetAllocators(&arena_allocator);
	
			isize const chuj_count = 5;
			char        *dummy     = alloc(sizeof(char) * chuj_count);
	
			dummy[0] = 'c';
			dummy[1] = 'h';
			dummy[2] = 'u';
			dummy[3] = 'j';
			dummy[4] = '\0';
	
			printf("%s\n", dummy);
	
			contextRemindAllocators();
	
			arenaDestroy(&arena);
		}
	
		{
			isize const heap_size = 32;
			i32         *ptr_2    = cast(i32*, alloc(sizeof(i32) * heap_size));
	
			Arena arena = arenaCreate(&(ArenaDescription) {
				.ptr_to_heap = ptr_2,
				.count       = sizeof(i32) * heap_size,
			});
	
			Allocator arena_allocator = arenaGetAllocator(&arena);
	
			contextSetAllocators(&arena_allocator);
	
			isize const chuj_count = 5;
			char        *dummy     = alloc(sizeof(char) * chuj_count);
	
			dummy[0] = 'c';
			dummy[1] = 'h';
			dummy[2] = 'u';
			dummy[3] = 'j';
			dummy[4] = '\0';
	
			printf("%s\n", dummy);
	
			contextRemindAllocators();
	
			arenaDestroy(&arena);
	
			free(ptr_2);
		}
	
		temporaryStorageReset();
		contextRemindAllocators();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Default allocator (wrapped), once again to test whether the default allocator has been reminded to `context`.\n");
	{
		f64 const start = timeStamp();
	
		isize *ptr = null;
	
		ptr = alloc(sizeof(isize) * 128);
		ptr = resize(ptr, sizeof(isize) * 192);
		free(ptr);
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Dynamic array with the default allocator.\n");
	{
		f64 const start = timeStamp();
	
		isize const iterations = 8192;
		isize const count      = 1024;
		for (isize i = 0; i < iterations; i += 1) {
			isize *dynamic_array = null;
	
			for (isize j = 0; j < count; j += 1) {
				darrAppend(dynamic_array, j * j * i);
			}
	
			darrFree(dynamic_array);
		}
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	
	printf("Dynamic array with the temporary-storage-based allocator.\n");
	{
		f64 const start = timeStamp();
	
		contextSetAllocators(&temporary_allocator);
	
		isize const iterations = 8192;
		isize const count      = 1024;
		for (isize i = 0; i < iterations; i += 1) {
			isize *dynamic_array = null;
	
			for (isize j = 0; j < count; j += 1) {
				darrAppend(dynamic_array, j * j * i);
			}
	
			temporaryStorageReset();
		}
	
		contextRemindAllocators();
	
		f64 const end = timeStamp();
	
		printf("%.9f\n", end - start);
	}
	
	printf("Temporary-storage-based allocator (wrapped), testing resize.\n");
	{
		f64 const start = timeStamp();
	
		contextSetAllocators(&temporary_allocator);
	
		isize *ptr   = null;
		isize *ptr_2 = null;
	
		ptr   = alloc(sizeof(isize) * 128);
		ptr   = resize(ptr, sizeof(isize) * 192);
		ptr_2 = alloc(sizeof(isize) * 128);
		ptr   = resize(ptr, sizeof(isize) * 256);
		free(ptr_2);
		free(ptr);
	
		f64 const end = timeStamp();
	
		temporaryStorageReset();
		contextRemindAllocators();
	
		printf("%.9f\n", end - start);
	}
 */